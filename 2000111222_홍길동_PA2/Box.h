#pragma once

using namespace std;

class Box
{

public:

	float speed; //박스의 속도

	float KE; //박스의 운동에너지
	float PE; //박스의 위치에너지
	float RE; //박스의 회전에너지

	float r; //top에서 봤을때 중점에서 꼭짓점까지의 길이

	//박스 중점의 위치
	float X;
	float Y;
	float Z;

	//박스 위치 변화량
	float diffX;
	float diffY;
	float diffZ;

	//중심에서부터의 모서리 방향으로의 현재 회전각
	float angleX;
	float angleY;
	float angleZ;

	//회전 변화량
	float angleDiffX;
	float angleDiffY;
	float angleDiffZ;
	
	//충돌상황 [0:충돌없음 충돌있음:3,6,9,12, 100(노회전 정면충돌)]
	int collisionPoint; 

	//충돌 후의 반발력
	float collisionValue;

	//박스의 초기값을 세팅
	void SetBox(
		float sizes, float collision,
		float x, float y, float z,
		float diffX, float diffY, float diffZ,
		float aDiffX, float aDiffY, float aDiffZ);


	void CollisionCheck();
};