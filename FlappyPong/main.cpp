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

	boolean Intersect3(olc::vf2d line_r1s, olc::vf2d line_r1e, olc::vf2d line_r2s, olc::vf2d line_r2e, olc::vf2d& hitLoc) {
		//From https://stackoverflow.com/a/1968345/8109619
		float p0_x = line_r1s.x;
		float p0_y = line_r1s.y;
		float p1_x = line_r1e.x;
		float p1_y = line_r1e.y;
		float p2_x = line_r2s.x;
		float p2_y = line_r2s.y;
		float p3_x = line_r2e.x;
		float p3_y = line_r2e.y;

		float s1_x, s1_y, s2_x, s2_y;
		s1_x = p1_x - p0_x;     s1_y = p1_y - p0_y;
		s2_x = p3_x - p2_x;     s2_y = p3_y - p2_y;

		float s, t;
		float h = -s2_x * s1_y + s1_x * s2_y;
		s = (-s1_y * (p0_x - p2_x) + s1_x * (p0_y - p2_y)) / h;
		t = (s2_x * (p0_y - p2_y) - s2_y * (p0_x - p2_x)) / h;

		if (s >= 0.0f && s <= 1.0f && t >= 0.0f && t <= 1.0f)
		{
			// Collision detected
			float i_x = p0_x + (t * s1_x);
			float i_y = p0_y + (t * s1_y);
			hitLoc = olc::vf2d(i_x, i_y); //save hit location in hitLoc
			return true;
		}

		return false; // No collision
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

		float balr = ballRadius * 0.6f;

		olc::vf2d hitLoc;
		boolean hitLeft = Intersect3(pos, { pos.x, pos.y + size.y }, ballPos, ballPos + (ballVel * fElapsedTime), hitLoc);
		boolean hitRight = Intersect3({ pos.x + size.x, pos.y }, { pos.x + size.x, pos.y + size.y }, ballPos, ballPos + (ballVel * fElapsedTime), hitLoc);
		boolean hitTop = Intersect3(pos, { pos.x + size.x, pos.y }, ballPos, ballPos + (ballVel * fElapsedTime), hitLoc);
		boolean hitBottom = Intersect3({ pos.x + size.x, pos.y + size.y }, { pos.x, pos.y + size.y }, ballPos, ballPos + (ballVel * fElapsedTime), hitLoc);

		if (hitLeft || hitRight)
			ballVel.x *= -1;

		if (hitTop || hitBottom)
			ballVel.y *= -1;

		//if (ballPos.x < pos.x + size.x + balr && ballPos.x > pos.x - balr &&
		//	ballPos.y < pos.y + size.y + balr && ballPos.y > pos.y - balr) //ball goes inside paddle
		//{
		//	ballVel.x *= -1;
		//}
		//if (ballPos.y > pos.y + size.y && ballPos.y < pos.y &&
		//	ballPos.x < pos.x && ballPos.x > pos.x + size.x) //ball touches paddle top or bottom
		//{
		//	ballVel.y *= -1;
		//}


		// Temporary variables to outputs
		opos = pos;
		ovel = vel;
		oacc = acc;
	}

	bool OnUserUpdate(float fElapsedTime) override
	{
		Clear(olc::VERY_DARK_BLUE);

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
		if (ballPos.x > ScreenWidth() + ballRadius * 2.5f || GetKey(olc::SPACE).bPressed)
		{
			resetBall();
		}

		// Rendering

		//Paddles
		FillRect(left.pos,  size, olc::WHITE);
		FillRect(right.pos, size, olc::WHITE);

		//Ball
		FillCircle(ballPos, ballRadius, olc::WHITE);

		//Middle Dashed Line
		DrawLine(ScreenWidth() / 2, 0, ScreenWidth() / 2, ScreenHeight(), olc::WHITE, 0x0FF00FF0);

		if (GetKey(olc::Key::ESCAPE).bPressed)
			return false;
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