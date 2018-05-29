#include "Box.h"


//초기값 지정
void Box::SetBox(
		float size, float collision,
		float x, float y, float z,
		float dX, float dY, float dZ,
		float adX, float adY, float adZ){

			//에너지
			KE =0.0f; 
			PE =0.0f;
			RE =0.0f;
			
			//각 축으로의 회전각
			angleX = 0.0f;
			angleY = 0.0f;
			angleZ = 0.0f;

			//크기
			r = size;
			
			//충돌계수
			collisionValue = collision;

			//초기위치
			X = x;
			Y = y;
			Z = z;

			//변화량
			diffX = dX;
			diffY = dY;
			diffZ = dZ;
			
			//회전 변화량
			angleDiffX = adX;
			angleDiffY= adY;
			angleDiffZ = adZ;
			
}



void Box::CollisionCheck(){
	
};
