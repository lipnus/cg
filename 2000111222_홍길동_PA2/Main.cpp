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
		glVertex3f(-20.0f, -1.0f, 20.0f);
		glVertex3f(-20.0f, -1.0f, -20.0f);
		glVertex3f(20.0f, -1.0f, -20.0f);
		glVertex3f(20.0f, -1.0f, 20.0f);
	glEnd();
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


//������ �ڽ��� �׷�����
void RenderBox(float x, float y, float z){

	float w = 6;
	float l = 2;
	float h = 1;

	glBegin(GL_QUADS);
				
		//1
		glNormal3f(0, 1, 0);
		glVertex3f(x, y, z);
		glVertex3f(x, y, z+l);
		glVertex3f(x+w, y, z+l);
		glVertex3f(x+w, y, z);

		//2
		glNormal3f(0, 0, 1);
		glVertex3f(x, y, z+l);
		glVertex3f(x+w, y, z+l);
		glVertex3f(x+w, y+h, z+l);
		glVertex3f(x, y+h, z+l);

		//3
		glNormal3f(0, 0, -1);
		glVertex3f(x, y, z);
		glVertex3f(x, y+h, z);
		glVertex3f(x+w, y+h, z);
		glVertex3f(x+w, y, z);

		//4
		glNormal3f(0, 1, 0);
		glVertex3f(x, y+h, z);
		glVertex3f(x, y+h, z+l);
		glVertex3f(x+w, y+h, z+l);
		glVertex3f(x+w, y+h, z);
		
		//5
		glNormal3f(-1,0, 0);
		glVertex3f(x, y, z);
		glVertex3f(x, y+h, z);
		glVertex3f(x, y+h, z+l);
		glVertex3f(x, y, z+l);

		//6
		glNormal3f(1, 0, 0);
		glVertex3f(x+w, y, z);
		glVertex3f(x+w, y+h, z);
		glVertex3f(x+w, y+h, z+l);
		glVertex3f(x+w, y, z+l);

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
void Rendering(void){
	
	// ȭ�� ���� Ŭ����
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	CameraSetting();

	RenderPlane();

	for(int i=0; i<100; i+=2){
		glColor3f(0.9f, 0.0f, 0.0f); RenderBox(0.0f, i, 0.0f);
		glColor3f(0.0f, 0.9f, 0.0f); RenderBox(0.0f, i, 2.0f);
		glColor3f(0.0f, 0.0f, 0.9f); RenderBox(0.0f, i, 4.0f);

		glRotatef(90, 0.0f, 1.0f, 0.0f);
		glTranslatef(-6.0f, 0.0f, 0.0f);
		glColor3f(1.0f, 1.0f, 0.0f); RenderBox(0.0f, i+1.0f, 0.0f);
		glColor3f(0.0f, 1.0f, 1.0f); RenderBox(0.0f, i+1.0f, 2.0f);
		glColor3f(1.0f, 0.0f, 1.0f); RenderBox(0.0f, i+1.0f, 4.0f);

		glTranslatef(6.0f, 0.0f, 0.0f);
		glRotatef(-90, 0.0f, 1.0f, 0.0f);
	}
	
	 




	
	////�Ž��� �ΰ��ϱ� �ι�
	//for(int i=0; i<2; i++){
	//	
	//	//�Ž��� ��ȭ ����
	//	meshControl(i);
	//	
	//	//�Ž��� �׸���
	//	RenderMesh(i);
	//}	

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

	//��ü����
	switch (key) {
		case '1':
			cout << "1��" << endl;
			selectedObj = 0;
			break;
		case '2':
			cout << "2��" << endl;
			selectedObj = 1;
			break;
	}

	//��ü�� ��Ʈ�Ѹ��
	switch (key) {
		case 't':
			cout << "�̵����" << endl;
			controlMode = CONTROLMODE_TRANS;
			break;
		case 'r':
			cout << "ȸ�����" << endl;
			controlMode = CONTROLMODE_ROTATE;
			break;
	}

	//�� ����
	switch (key){
		case 'x':
			controlAxis = CONTOLAXIS_X;
			break;
		case 'y':
			controlAxis = CONTOLAXIS_Y;
			break;
		case 'z':
			controlAxis = CONTOLAXIS_Z;
			break;
	}

	//curring face
	switch(key){
		case 'b':
			if(faceCurringOn){ 
				cout << "BackFaceCulling OFF" << endl;
				faceCurringOn = false; 
			}else{ 
				cout << "BackFaceCulling ON" << endl;
				faceCurringOn = true; 
			}
			break;

		case '3':
			if(DepthTest){
				cout << "DepthTest ON" << endl;
				glEnable(GL_DEPTH_TEST);
			}
			else{
				cout << "DepthTest OFF" << endl;
				glDisable(GL_DEPTH_TEST);
			}
			DepthTest = !DepthTest;
			break;
	}

	glutPostRedisplay();
}

void EventHandlingAndLoop()
{
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
