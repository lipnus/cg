#include "Box.h"


//�ʱⰪ ����
void Box::SetBox(
		float size, float collision,
		float x, float y, float z,
		float dX, float dY, float dZ,
		float adX, float adY, float adZ){

			//������
			KE =0.0f; 
			PE =0.0f;
			RE =0.0f;
			
			//�� �������� ȸ����
			angleX = 0.0f;
			angleY = 0.0f;
			angleZ = 0.0f;

			//ũ��
			r = size;
			
			//�浹���
			collisionValue = collision;

			//�ʱ���ġ
			X = x;
			Y = y;
			Z = z;

			//��ȭ��
			diffX = dX;
			diffY = dY;
			diffZ = dZ;
			
			//ȸ�� ��ȭ��
			angleDiffX = adX;
			angleDiffY= adY;
			angleDiffZ = adZ;
			
}



void Box::CollisionCheck(){
	
};
