#pragma once

using namespace std;

class Box
{

public:

	float speed; //�ڽ��� �ӵ�

	float KE; //�ڽ��� �������
	float PE; //�ڽ��� ��ġ������
	float RE; //�ڽ��� ȸ��������

	float r; //top���� ������ �������� ������������ ����

	//�ڽ� ������ ��ġ
	float X;
	float Y;
	float Z;

	//�ڽ� ��ġ ��ȭ��
	float diffX;
	float diffY;
	float diffZ;

	//�߽ɿ��������� �𼭸� ���������� ���� ȸ����
	float angleX;
	float angleY;
	float angleZ;

	//ȸ�� ��ȭ��
	float angleDiffX;
	float angleDiffY;
	float angleDiffZ;
	
	//�浹��Ȳ [0:�浹���� �浹����:3,6,9,12, 100(��ȸ�� �����浹)]
	int collisionPoint; 

	//�浹 ���� �ݹ߷�
	float collisionValue;

	//�ڽ��� �ʱⰪ�� ����
	void SetBox(
		float sizes, float collision,
		float x, float y, float z,
		float diffX, float diffY, float diffZ,
		float aDiffX, float aDiffY, float aDiffZ);


	void CollisionCheck();
};