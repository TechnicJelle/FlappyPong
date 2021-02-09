#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"

float PI = 3.14159f;

float random(float Max)
{
	return (float(rand()) / float(RAND_MAX)) * Max;
}

float random(float Min, float Max)
{
	return ((float(rand()) / float(RAND_MAX)) * (Max - Min)) + Min;
}

class FlappyPong : public olc::PixelGameEngine
{
public:
	FlappyPong()
	{
		sAppName = "FlappyPong";
	}

private:
	float gravity = 500.0f;

	olc::vf2d size; // Both are the same size

	struct paddle
	{
		olc::vf2d pos;
		float vel = 0.0f;
		float acc = 0.0f;
	};

	paddle left, right;

	olc::vf2d ballPos;
	olc::vf2d ballVel;
	int ballRadius = 5;

protected:
	void resetBall()
	{
		ballPos = { ScreenWidth() / 2.0f, ScreenHeight() / 2.0f };

		float fAngle = random(-PI / 4.0f, PI / 4.0f); //Random angle to the right
		fAngle += PI * round(random(1.0f)); //Randomly offset it to the left side or not

		float ballVelMult = 100.0f;
		ballVel = { ballVelMult * cos(fAngle), ballVelMult * sin(fAngle) };
	}

	bool OnUserCreate() override
	{
		size = { 10.0f, 50.0f }; // Both are the same size

		left.pos = { size.x * 0.5f,	ScreenHeight() / 2 - size.y / 2 };
		right.pos = { ScreenWidth() - size.x * 1.5f,	ScreenHeight() / 2 - size.y / 2 };

		resetBall();

		return true;
	}

	void update(olc::vf2d ipos, float ivel, float iacc, olc::Key key, float fElapsedTime, olc::vf2d& opos, float& ovel, float& oacc)
	{
		// Inputs to temporary variables
		olc::vf2d pos = ipos;
		float vel = ivel;
		float acc = iacc;

		// Physics
		if (GetKey(key).bPressed && vel > 0.0f)
		{
			acc = 0.0f;
			vel = -gravity / 3.0f;
		}
		else
			acc += 2 * gravity * fElapsedTime;

		if (acc >= gravity)
			acc = gravity;

		vel += acc * fElapsedTime;
		pos.y = std::clamp(pos.y + vel * fElapsedTime, 0.0f, ScreenHeight() - size.y);

		if (ballPos.x > pos.x - ballRadius * 0.5f)
		{
			ballVel.x *= -1;
		}


		// Temporary variables to outputs
		opos = pos;
		ovel = vel;
		oacc = acc;
	}

	bool OnUserUpdate(float fElapsedTime) override
	{
		// Physics

		//Paddles
		update(left.pos,  left.vel,  left.acc,  olc::Key::LSHIFT, fElapsedTime, left.pos,  left.vel,  left.acc);
		update(right.pos, right.vel, right.acc, olc::Key::RSHIFT, fElapsedTime, right.pos, right.vel, right.acc);

		//Ball
		ballPos += ballVel * fElapsedTime;
		//top
		if (ballPos.y > (float)(ScreenHeight() - ballRadius))
		{
			ballVel.y *= -1;
			ballPos.y = (float)(ScreenHeight() - ballRadius);
		}
		//bottom
		if (ballPos.y < (float)ballRadius)
		{
			ballVel.y *= -1;
			ballPos.y = (float)ballRadius;
		}
		//left
		if (ballPos.x < ballRadius * -2.5f)
		{
			resetBall();
		}
		//right
		if (ballPos.x > ScreenWidth() + ballRadius * 2.5f)
		{
			resetBall();
		}

		// Rendering

		Clear(olc::DARK_BLUE);

		//Paddles
		FillRect(left.pos,  size, olc::WHITE);
		FillRect(right.pos, size, olc::WHITE);

		//Ball
		FillCircle(ballPos, ballRadius, olc::WHITE);

		//Middle Dashed Line
		int segLength = ScreenHeight() / 42;
		for (int i = 0; i < ScreenHeight(); i += segLength * 2)
			DrawLine(ScreenWidth() / 2, (int)std::round(i + segLength * 0.5f), ScreenWidth() / 2, (int)std::round(i + segLength * 1.5f), olc::WHITE);

		return true;
	}
};

int main()
{
	FlappyPong demo;
	if (demo.Construct(470, 240, 4, 4))
		demo.Start();
	return 0;
}