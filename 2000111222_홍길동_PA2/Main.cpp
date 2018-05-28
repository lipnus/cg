#include "VECTOR.h"
#include "Face.h"
#include "Mesh.h"
#include "Init.h"


using namespace std;

bool DepthTest = true;
Mesh mesh[2];

//ī�޶� ����
float CamDiffX = 0;
float CamDiffY = 0;
float CamDiffZ = 0;
float angle = 0; //ȸ��

//� ��ü����
int selectedObj = 0;

//�̵��� ���� ȸ���Ұ���
const int CONTROLMODE_TRANS = 0;
const int CONTROLMODE_ROTATE = 1;
int controlMode = CONTROLMODE_TRANS; //t�� r�� ��ȯ

//��������� �̵�, ȸ�� �� ������
const int CONTOLAXIS_X = 0;
const int CONTOLAXIS_Y = 1;
const int CONTOLAXIS_Z = 2;
int controlAxis = CONTOLAXIS_X;

//��ü�� �̵� ��ȭ��
GLfloat objDiffX[2] = {0,0};
GLfloat objDiffY[2] = {0,0};
GLfloat objDiffZ[2] = {0,0};

//��ü�� ���� ��ȭ��
GLfloat objAngleDiffX[2] = {0,0};
GLfloat objAngleDiffY[2] = {0,0};
GLfloat objAngleDiffZ[2] = {0,0};

//���� ���̽� �ø�
bool faceCurringOn = false;

const float PI = 3.1415926535897;

//���콺Ŀ�� ��ǥ
int mouse_x = 0;
int mouse_y = 0;

GLfloat time = 0.0f;
GLfloat boxSpeed[10] = { 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f};
GLfloat boxKE[10] = { 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f};



int which = 0;

//�ڽ�ũ��
GLfloat r[10]={5.0f, 4.0f, 5.0f, 5.0f, 5.0f, 5.0f, 5.0f, 5.0f, 5.0f, 5.0f};

//�ڽ���ġ
GLfloat boxX[10]={ 0.0f, 15.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f};
GLfloat boxY[10]={ 30.0f, 20.0f, 30.0f, 30.0f, 30.0f, 30.0f, 30.0f, 30.0f, 30.0f, 30.0f };
GLfloat boxZ[10]={ 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f};
 
GLfloat angleX[10] = { 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f};
GLfloat angleY[10] = { 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f};
GLfloat angleZ[10] = { 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f};

int collisionPoint[10]={ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }; //0:�浹���� �浹����:3,6,9,12, 100(��ȸ�� �����浹)
void checkCollision(int n);


//GLfloat angleTh = 45.0f;
//GLfloat anglePi = 0.0f;


//�Ž����Ϸε�
void MeshLoad()
{
	//To do
	mesh[0].LoadMesh("lego.obj");
	mesh[1].LoadMesh("torus.obj");
}

//�Ž��� �븻���Ͱ��
void ComputeNormal()
{
	//To do
	for(int i=0; i < 2; i++){
		mesh[i].ComputeFaceNormal(); //Face Normal ���
		mesh[i].FindNeighborFaceArray(); //Vertex�� ���� face
		mesh[i].ComputeVertexNormal(); //Vertex�� Normal ���
	}
}

//�Ž��� ��ȭ�� ����
void meshControl(int nowDraw){

	glTranslatef(objDiffX[nowDraw], objDiffY[nowDraw], objDiffZ[nowDraw]);

	glRotatef(objAngleDiffX[nowDraw], 1.0f, 0.0f, 0.0f);
	glRotatef(objAngleDiffY[nowDraw], 0.0f, 1.0f, 0.0f);
	glRotatef(objAngleDiffZ[nowDraw], 0.0f, 0.0f, 1.0f);
}

//Load�ϰ� ����� Mesh�� ������ �̿��Ͽ� �ﰢ���� �׸���
void RenderMesh(int whichMesh)
{
	glBegin(GL_TRIANGLES);
	for(int i=0; i < mesh[whichMesh].faceArray.size(); i++){	

		//face normal
		glNormal3f(mesh[whichMesh].faceArray[i].normal.x, mesh[whichMesh].faceArray[i].normal.y, mesh[whichMesh].faceArray[i].normal.z);

		//������ �ſ� �� ������
		if(selectedObj == whichMesh){
			glColor3f(1.0f, 0.0f, 0.0f);
		}else{
			glColor3f(1.0f, 1.0f, 1.0f);
		}


		//[faceCulling]
		//==============================================================================
		
		//�ո鸸 �׸���
		if(faceCurringOn){

			//ī�޶��� �ü�
			VECTOR3D camEye = VECTOR3D((22.0f - CamDiffY)*sin(angle*PI/180), -(22.0f - CamDiffY), -(22.0f - CamDiffZ)*cos(angle*PI/180));  
			
			//������ ��츸 �׷��ش�
			if( camEye.InnerProduct(mesh[whichMesh].faceArray[i].normal) < 0){
				//face�� �����ϴ� 3���� ������ ������
				Vertex v0 = mesh[whichMesh].vertexArray[mesh[whichMesh].faceArray[i].vertex0];
				Vertex v1 = mesh[whichMesh].vertexArray[mesh[whichMesh].faceArray[i].vertex1];
				Vertex v2 = mesh[whichMesh].vertexArray[mesh[whichMesh].faceArray[i].vertex2];

				//glNormal3f: �����ÿ� ����� �������� ����
				//glVertex3f: �ﰢ���� ��ġ ����

				//vertex normal
				glNormal3f(v0.normal.x, v0.normal.y, v0.normal.z);
				glVertex3f(v0.position.x, v0.position.y, v0.position.z);
				glNormal3f(v1.normal.x, v1.normal.y, v1.normal.z);
				glVertex3f(v1.position.x, v1.position.y, v1.position.z);
				glNormal3f(v2.normal.x, v2.normal.y, v2.normal.z);
				glVertex3f(v2.position.x, v2.position.y, v2.position.z);
			}else{
				//�޸��� ����
			}
		}
		
		//������ ���. �� �׸��� DEPTH_TEST�� ó���ϵ��� �Ѵ�
		else{

			//face�� �����ϴ� 3���� ������ ������
			Vertex v0 = mesh[whichMesh].vertexArray[mesh[whichMesh].faceArray[i].vertex0];
			Vertex v1 = mesh[whichMesh].vertexArray[mesh[whichMesh].faceArray[i].vertex1];
			Vertex v2 = mesh[whichMesh].vertexArray[mesh[whichMesh].faceArray[i].vertex2];

			//glNormal3f: �����ÿ� ����� �������� ����
			//glVertex3f: �ﰢ���� ��ġ ����

			//vertex normal
			glNormal3f(v0.normal.x, v0.normal.y, v0.normal.z);
			glVertex3f(v0.position.x, v0.position.y, v0.position.z);
			glNormal3f(v1.normal.x, v1.normal.y, v1.normal.z);
			glVertex3f(v1.position.x, v1.position.y, v1.position.z);
			glNormal3f(v2.normal.x, v2.normal.y, v2.normal.z);
			glVertex3f(v2.position.x, v2.position.y, v2.position.z);
		}
		
		//==============================================================================



	}//for(�޽��׸���)
	glEnd();
}

//���콺 ��Ʈ��
void Mouse(int mouse_event, int state, int x, int y)
{
	//To Do
	glutPostRedisplay();
}

//���콺 Ŀ�� �̵�
void Motion(int x, int y)
{
	//To Do
	//�ּ��� ���� ������ ��ũ�������� ���콺Ŀ���� �̵������� �ǹ�
	
	if(controlMode == CONTROLMODE_TRANS){
		
		if(controlAxis == CONTOLAXIS_X){
			if(mouse_x < x){//������
				objDiffX[selectedObj] += 1;
			}else{ //����
				objDiffX[selectedObj] -= 1;
			}
		}else if(controlAxis == CONTOLAXIS_Z) {
			if(mouse_y > y){ //����
				objDiffZ[selectedObj] -= 1;
			}else{ //�Ʒ���
				objDiffZ[selectedObj] += 1;
			}
		}else if(controlAxis == CONTOLAXIS_Y){
			if(mouse_y > y){ //����
				objDiffY[selectedObj] += 1;
			}else{ //�Ʒ���
				objDiffY[selectedObj] -= 1;
			}
		}

	}else if(controlMode == CONTROLMODE_ROTATE){
	
		if(controlAxis == CONTOLAXIS_X){
			if(mouse_y < y){//����
				objAngleDiffX[selectedObj] += 15;
			}else{ //�Ʒ���
				objAngleDiffX[selectedObj] -= 15;
			}
		}else if(controlAxis == CONTOLAXIS_Z) {
			if(mouse_x < x){ //������
				objAngleDiffZ[selectedObj] -= 15;
			}else{ //����
				objAngleDiffZ[selectedObj] += 15;
			}
		}else if(controlAxis == CONTOLAXIS_Y){
			if(mouse_x < x){ //������
				objAngleDiffY[selectedObj] += 15;
			}else{ //����
				objAngleDiffY[selectedObj] -= 15;
			}
		}
	}

	//����� ��ǥ ������Ʈ
	mouse_x = x;
	mouse_y = y;

	glutPostRedisplay();
}

//�Ƕ��� �׸���
void RenderPlane()
{
	glBegin(GL_QUADS);
		glColor3f(0.0f, 1.0f, 0.0f);
		glNormal3f(0, 1, 0);
		glVertex3f(-50.0f, 0.0f, 50.0f);
		glVertex3f(-50.0f, 0.0f, -50.0f);
		glVertex3f(50.0f, 0.0f, -50.0f);
		glVertex3f(50.0f, 0.0f, 50.0f);
	glEnd();
}

//�ڽ�
void RenderBox(float x, float y, float z, int n){
	
 	float h = r[n]*1.41421f/2;

	glBegin(GL_QUADS);
				
		//�ٴ�
		glNormal3f(0, -1, 0);
		glColor3f(0.0f, 1.0f, 1.0f);
		glVertex3f(x+0, y-h, z+r[n]);
		glVertex3f(x+r[n], y-h, z+0);
		glVertex3f(x+0, y-h, z-r[n]);
		glVertex3f(x-r[n], y-h, z+0);

		//õ��
		glNormal3f(0, 1, 0);
		glColor3f(1.0f, 0.0f, 0.0f);
		glVertex3f(x+0, y+h, z+r[n]);
		glVertex3f(x+r[n], y+h, z+0);
		glVertex3f(x+0, y+h, z-r[n]);
		glVertex3f(x-r[n], y+h, z+0);

		//2�� ��
		glNormal3f(1, 0, -1);
		glColor3f(0.0f, 0.0f, 0.0f);
		glVertex3f(x+0, y+h, z-r[n]);
		glVertex3f(x+0, y-h, z-r[n]);
		glVertex3f(x+r[n], y-h, z+0);
		glVertex3f(x+r[n], y+h, z+0);
		glColor3f(0.9f, 0.0f, 0.0f);

		//4�� ��
		glNormal3f(1, 0, 1);
		glColor3f(-0.0f, 0.0f, 1.0f);
		glVertex3f(x+r[n], y+h, z+0);
		glVertex3f(x+r[n], y-h, z+0);
		glVertex3f(x+0, y-h, z+r[n]);
		glVertex3f(x+0, y+h, z+r[n]);
		
		//8�� ��
		glNormal3f(-1, 0, 1);
		glColor3f(-0.0f, 0.0f, 1.0f);
		glVertex3f(x-r[n], y+h, z+0);
		glVertex3f(x-r[n], y-h, z+0);
		glVertex3f(x+0, y-h, z+r[n]);
		glVertex3f(x+0, y+h, z+r[n]);

		//11�ú�
		glNormal3f(-1, 0, -1);
		glColor3f(-0.0f, 0.0f, 1.0f);
		glVertex3f(x+0, y+h, z-r[n]);
		glVertex3f(x+0, y-h, z-r[n]);
		glVertex3f(x-r[n], y-h, z+0);
		glVertex3f(x-r[n], y+h, z+0);

	glEnd();
}

//���� ����
void CameraSetting(void){
	// ȭ���� ����� �ٶ󺸱� ���� ī�޶� ȸ�� �� �̵�
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	//���ؽ���
	gluLookAt(
		0.0f, 25.0f, 30.0f,
		0.0f, 10.0f, 0.0f,
		0.0f, 1.0f, 0.0f);

	glTranslatef(0.0f, CamDiffY, CamDiffZ); //���� Ȯ�����
	glRotatef(angle, 0.0f, 1.0f, 0.0f); //���� ȸ��
}

//�׸���
void Rendering(){
	
	// ȭ�� ���� Ŭ����
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	CameraSetting();

	RenderPlane();
	
	for(int n=0; n<2; n++){
		glPushMatrix();
		glTranslatef(boxX[n], boxY[n], boxZ[n]);


		//�浹 �� ���� ����
		if(boxKE[n] < 0){
			glTranslatef( -r[n], 0.0f, 0.0f);
		}

		//��ȭ
		glRotatef(angleY[n], 0.0f, 1.0f, 0.0f);
		glRotatef(angleX[n], 1.0f, 0.0f, 0.0f);
		glRotatef(angleZ[n], 0.0f, 0.0f, 1.0f);
	
		//�浹 �� ���� ����
		if(boxKE[n] < 0){
			glTranslatef( r[n], 0.0f, 0.0f);
		}

		glColor3f(0.9f, 0.0f, 0.0f); 
		RenderBox(0.0f, 0.0f, 0.0f, n);
		glPopMatrix();
	}
	
	
	// back ���ۿ� �������� �� swap
	glutSwapBuffers();
}

void Reshape(int w, int h)
{
	// ����Ʈ ����
	glViewport(0, 0, w, h);

	// ���� ���� ���
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45, (float)w / h, 0.1, 500);

	// �𵨺� ��Ʈ���� �ʱ�ȭ
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void Keyboard(unsigned char key, int x, int y)
{
	//ī�޶� ����
	switch (key){
	case 'w':
		CamDiffY = CamDiffY + 1;
		CamDiffZ = CamDiffZ + 1;
		break;
	case 's':
		CamDiffY = CamDiffY - 1;
		CamDiffZ = CamDiffZ - 1;
		break;
	case 'a':
		angle = angle + 2;
		break;
	case 'd':
		angle = angle - 2;
		break;
	default:
		break;
	}

	//�浹
	switch (key){
		case 'h':
			angleX[which] += 1.0f;
			break;
		case 'j':
			angleX[which] -= 1.0f;
			break;
		case 'k':
			angleY[which] += 1.0f;
			break;
		case 'l':
			angleY[which] -= 1.0f;
			break;
		case 'n':
			angleZ[which] += 1.0f;
			break;
		case 'm':
			angleZ[which] -= 1.0f;
			break;

		case 'o':
			boxY[which] += 1.0f;
			break;
		case 'p':
			boxY[which] -= 1.0f;
			break;
		
		case 'q': //����
			for(int n=0; n<2; n++){
				boxY[n] = 30.0f;
				boxKE[n] = 1.0f;
				angleX[n] = 0;
				angleY[n] = 0;
				angleZ[n] = 0;
				time = 0;
				boxSpeed[n] = 0;
				collisionPoint[n] = 0;
			}
		break;
	}


	checkCollision(0);
	glutPostRedisplay();
}

//�浹�� üũ�Ѵ�(���° ������ü���� ������־�� �Ѵ�)
void checkCollision(int n){

	float startAngle = atan(0.70710678118); //������ ������ü �𼭸��� �̷�� ��
	float edgeLen = sqrt(r[n]*r[n] + r[n]*r[n] + r[n]*r[n]);

	if(boxSpeed[n] > 0){ //�浹�� ���������� ����
		if( (boxY[n] - edgeLen*sin( startAngle + angleX[n]/180*PI )) < 0){
			collisionPoint[n]=6;
			cout<< "6���浹" << endl;
		}else if( (boxY[n] - edgeLen*sin( startAngle - angleX[n]/180*PI )) < 0 ){
			collisionPoint[n]=12;
			cout << "12�� �浹" << endl;
		}else if( (boxY[n] - edgeLen*sin( startAngle + angleZ[n]/180*PI )) < 0){
			collisionPoint[n]=3;
			cout<< "3���浹" << endl;
		}else if( (boxY[n] - edgeLen*sin( startAngle - angleZ[n]/180*PI )) < 0){
			collisionPoint[n]=9;
			cout<< "9���浹" << endl;
		}
	}
}

void TimerFunc(int value)
{

	checkCollision(0);
	checkCollision(1);

	//int n=0;

	for(int n=0; n<2; n++){
		//0.01�ʸ��� ����
		if(boxKE[n] > 0){

			//�ϰ� �� ��ȭ��
			angleX[0] += 0.03;
			angleX[1] -= 0.01;

			angleY[0] += 0.15;
			angleY[1] -= 0.25;

			angleZ[n] += 0.02;

			/*
			boxX += 0.001;
			boxZ += 0.001;*/

			boxSpeed[n] += 0.001f; //���ǵ尡 ������ ������ �ö󰡴µ� �̰� ���ӵ�(g)�� ������ �Ѵ�
		
			//�ٴڸ�� �浹
			if(collisionPoint[n] != 0 && boxSpeed > 0){ 

				if(boxSpeed[n] > 0.02){ //�ٽ� Ƣ������� ���
					cout<< "[" << n << "] ��! �ӵ�:" << boxSpeed[n] << endl;
					boxSpeed[n] = boxSpeed[n] * 0.5;
					boxSpeed[n] = boxSpeed[n] * (-1);
					collisionPoint[n]=0;
				}else{ //��
					cout<< "[" << n << "] ����! �ӵ�:" << boxSpeed[n] << endl;
					boxKE[n] = 0;
					boxSpeed[n] = 0;
					cout << "[" << n << "] ����" << endl;
				}
			}
		}else{ //������Ȳ���� �������� ����(kE = 0) 
			if(angleX[n] > 1){
				//cout << "����" << endl;
				angleX[n] -= 0.4f*time*time*time;
				boxY[n] -= 0.01f;
				boxX[n] += 0.02f;
				boxZ[n] += 0.02f;

			}else{
				//cout << "�����Ϸ�" << endl;
			}
		}//KE
	}//for
	
	

	boxY[0] -= 0.5f * time * time *70* boxSpeed[0]; // 1/2a t^2
	boxY[1] -= 0.5f * time * time *40* boxSpeed[1]; // 1/2a t^2

	time = time + 0.002f;

	glutPostRedisplay();
	glutTimerFunc(1, TimerFunc, 1);
}

void EventHandlingAndLoop()
{
	glutTimerFunc(1, TimerFunc, 1); //Ÿ�̸�

	glutKeyboardFunc(Keyboard);  // Ű���� �Է� ó�� Callback �Լ� ���
	glutDisplayFunc(Rendering);  // ��ȯ�� ���� ���� Rendering Callback �Լ� ���
	glutReshapeFunc(Reshape);    // ������ â ũ�Ⱑ �ٲ������ ȣ��Ǵ� Callback �Լ� ���
	glutMouseFunc(Mouse);        // ���콺 Ŭ�� ó�� Callback �Լ� ���
	glutMotionFunc(Motion);      // ���콺 ������ ó�� Callback �Լ� ���

	glutMainLoop(); // ��ϵ� callback �Լ��� �ݺ��Ͽ� ȣ��
}



int main(int argc, char** argv)
{
	Initialize(argc, argv);			  // ������ ����, ���� ����

	MeshLoad();       //To Do
	ComputeNormal();  //To Do
	EventHandlingAndLoop();      // Event Handling �� Loop

	// ���� ���� ������ ��� 0�� ������
	return 0;
}
