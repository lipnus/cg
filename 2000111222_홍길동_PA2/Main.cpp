#include "VECTOR.h"
#include "Face.h"
#include "Mesh.h"
#include "Init.h"


using namespace std;

bool DepthTest = true;
Mesh mesh[2];

//카메라 시점
float CamDiffX = 0;
float CamDiffY = 0;
float CamDiffZ = 0;
float angle = 0; //회전

//어떤 물체인지
int selectedObj = 0;

//이동할 건지 회전할건지
const int CONTROLMODE_TRANS = 0;
const int CONTROLMODE_ROTATE = 1;
int controlMode = CONTROLMODE_TRANS; //t와 r로 전환

//어느축으로 이동, 회전 할 것인지
const int CONTOLAXIS_X = 0;
const int CONTOLAXIS_Y = 1;
const int CONTOLAXIS_Z = 2;
int controlAxis = CONTOLAXIS_X;

//물체의 이동 변화량
GLfloat objDiffX[2] = {0,0};
GLfloat objDiffY[2] = {0,0};
GLfloat objDiffZ[2] = {0,0};

//물체의 각도 변화량
GLfloat objAngleDiffX[2] = {0,0};
GLfloat objAngleDiffY[2] = {0,0};
GLfloat objAngleDiffZ[2] = {0,0};

//셀프 페이스 컬링
bool faceCurringOn = false;

const float PI = 3.1415926535897;

//마우스커서 좌표
int mouse_x = 0;
int mouse_y = 0;

GLfloat time = 0.0f;
GLfloat boxSpeed[10] = { 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f};
GLfloat boxKE[10] = { 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f};



int which = 0;

//박스크기
GLfloat r[10]={5.0f, 4.0f, 5.0f, 5.0f, 5.0f, 5.0f, 5.0f, 5.0f, 5.0f, 5.0f};

//박스위치
GLfloat boxX[10]={ 0.0f, 15.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f};
GLfloat boxY[10]={ 30.0f, 20.0f, 30.0f, 30.0f, 30.0f, 30.0f, 30.0f, 30.0f, 30.0f, 30.0f };
GLfloat boxZ[10]={ 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f};
 
GLfloat angleX[10] = { 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f};
GLfloat angleY[10] = { 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f};
GLfloat angleZ[10] = { 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f};

int collisionPoint[10]={ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }; //0:충돌없음 충돌있음:3,6,9,12, 100(노회전 정면충돌)
void checkCollision(int n);


//GLfloat angleTh = 45.0f;
//GLfloat anglePi = 0.0f;


//매쉬파일로드
void MeshLoad()
{
	//To do
	mesh[0].LoadMesh("lego.obj");
	mesh[1].LoadMesh("torus.obj");
}

//매쉬의 노말벡터계산
void ComputeNormal()
{
	//To do
	for(int i=0; i < 2; i++){
		mesh[i].ComputeFaceNormal(); //Face Normal 계산
		mesh[i].FindNeighborFaceArray(); //Vertex의 인접 face
		mesh[i].ComputeVertexNormal(); //Vertex의 Normal 계산
	}
}

//매쉬에 변화량 적용
void meshControl(int nowDraw){

	glTranslatef(objDiffX[nowDraw], objDiffY[nowDraw], objDiffZ[nowDraw]);

	glRotatef(objAngleDiffX[nowDraw], 1.0f, 0.0f, 0.0f);
	glRotatef(objAngleDiffY[nowDraw], 0.0f, 1.0f, 0.0f);
	glRotatef(objAngleDiffZ[nowDraw], 0.0f, 0.0f, 1.0f);
}

//Load하고 계산한 Mesh의 정보를 이용하여 삼각형을 그린다
void RenderMesh(int whichMesh)
{
	glBegin(GL_TRIANGLES);
	for(int i=0; i < mesh[whichMesh].faceArray.size(); i++){	

		//face normal
		glNormal3f(mesh[whichMesh].faceArray[i].normal.x, mesh[whichMesh].faceArray[i].normal.y, mesh[whichMesh].faceArray[i].normal.z);

		//선택한 거에 색 입히기
		if(selectedObj == whichMesh){
			glColor3f(1.0f, 0.0f, 0.0f);
		}else{
			glColor3f(1.0f, 1.0f, 1.0f);
		}


		//[faceCulling]
		//==============================================================================
		
		//앞면만 그린다
		if(faceCurringOn){

			//카메라의 시선
			VECTOR3D camEye = VECTOR3D((22.0f - CamDiffY)*sin(angle*PI/180), -(22.0f - CamDiffY), -(22.0f - CamDiffZ)*cos(angle*PI/180));  
			
			//정면인 경우만 그려준다
			if( camEye.InnerProduct(mesh[whichMesh].faceArray[i].normal) < 0){
				//face를 구성하는 3개의 정점을 가져옴
				Vertex v0 = mesh[whichMesh].vertexArray[mesh[whichMesh].faceArray[i].vertex0];
				Vertex v1 = mesh[whichMesh].vertexArray[mesh[whichMesh].faceArray[i].vertex1];
				Vertex v2 = mesh[whichMesh].vertexArray[mesh[whichMesh].faceArray[i].vertex2];

				//glNormal3f: 라이팅에 사용할 법선벡터 설정
				//glVertex3f: 삼각형의 위치 설정

				//vertex normal
				glNormal3f(v0.normal.x, v0.normal.y, v0.normal.z);
				glVertex3f(v0.position.x, v0.position.y, v0.position.z);
				glNormal3f(v1.normal.x, v1.normal.y, v1.normal.z);
				glVertex3f(v1.position.x, v1.position.y, v1.position.z);
				glNormal3f(v2.normal.x, v2.normal.y, v2.normal.z);
				glVertex3f(v2.position.x, v2.position.y, v2.position.z);
			}else{
				//뒷면은 생깜
			}
		}
		
		//기존의 경우. 다 그리고 DEPTH_TEST가 처리하도록 한다
		else{

			//face를 구성하는 3개의 정점을 가져옴
			Vertex v0 = mesh[whichMesh].vertexArray[mesh[whichMesh].faceArray[i].vertex0];
			Vertex v1 = mesh[whichMesh].vertexArray[mesh[whichMesh].faceArray[i].vertex1];
			Vertex v2 = mesh[whichMesh].vertexArray[mesh[whichMesh].faceArray[i].vertex2];

			//glNormal3f: 라이팅에 사용할 법선벡터 설정
			//glVertex3f: 삼각형의 위치 설정

			//vertex normal
			glNormal3f(v0.normal.x, v0.normal.y, v0.normal.z);
			glVertex3f(v0.position.x, v0.position.y, v0.position.z);
			glNormal3f(v1.normal.x, v1.normal.y, v1.normal.z);
			glVertex3f(v1.position.x, v1.position.y, v1.position.z);
			glNormal3f(v2.normal.x, v2.normal.y, v2.normal.z);
			glVertex3f(v2.position.x, v2.position.y, v2.position.z);
		}
		
		//==============================================================================



	}//for(메쉬그리기)
	glEnd();
}

//마우스 컨트롤
void Mouse(int mouse_event, int state, int x, int y)
{
	//To Do
	glutPostRedisplay();
}

//마우스 커서 이동
void Motion(int x, int y)
{
	//To Do
	//주석에 쓰인 방향은 스크린에서의 마우스커서의 이동방향을 의미
	
	if(controlMode == CONTROLMODE_TRANS){
		
		if(controlAxis == CONTOLAXIS_X){
			if(mouse_x < x){//오른쪽
				objDiffX[selectedObj] += 1;
			}else{ //왼쪽
				objDiffX[selectedObj] -= 1;
			}
		}else if(controlAxis == CONTOLAXIS_Z) {
			if(mouse_y > y){ //위로
				objDiffZ[selectedObj] -= 1;
			}else{ //아래로
				objDiffZ[selectedObj] += 1;
			}
		}else if(controlAxis == CONTOLAXIS_Y){
			if(mouse_y > y){ //위로
				objDiffY[selectedObj] += 1;
			}else{ //아래로
				objDiffY[selectedObj] -= 1;
			}
		}

	}else if(controlMode == CONTROLMODE_ROTATE){
	
		if(controlAxis == CONTOLAXIS_X){
			if(mouse_y < y){//위로
				objAngleDiffX[selectedObj] += 15;
			}else{ //아래로
				objAngleDiffX[selectedObj] -= 15;
			}
		}else if(controlAxis == CONTOLAXIS_Z) {
			if(mouse_x < x){ //오른쪽
				objAngleDiffZ[selectedObj] -= 15;
			}else{ //왼쪽
				objAngleDiffZ[selectedObj] += 15;
			}
		}else if(controlAxis == CONTOLAXIS_Y){
			if(mouse_x < x){ //오른쪽
				objAngleDiffY[selectedObj] += 15;
			}else{ //왼쪽
				objAngleDiffY[selectedObj] -= 15;
			}
		}
	}

	//저장된 좌표 업데이트
	mouse_x = x;
	mouse_y = y;

	glutPostRedisplay();
}

//판때기 그리기
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

//박스
void RenderBox(float x, float y, float z, int n){
	
 	float h = r[n]*1.41421f/2;

	glBegin(GL_QUADS);
				
		//바닥
		glNormal3f(0, -1, 0);
		glColor3f(0.0f, 1.0f, 1.0f);
		glVertex3f(x+0, y-h, z+r[n]);
		glVertex3f(x+r[n], y-h, z+0);
		glVertex3f(x+0, y-h, z-r[n]);
		glVertex3f(x-r[n], y-h, z+0);

		//천장
		glNormal3f(0, 1, 0);
		glColor3f(1.0f, 0.0f, 0.0f);
		glVertex3f(x+0, y+h, z+r[n]);
		glVertex3f(x+r[n], y+h, z+0);
		glVertex3f(x+0, y+h, z-r[n]);
		glVertex3f(x-r[n], y+h, z+0);

		//2시 벽
		glNormal3f(1, 0, -1);
		glColor3f(0.0f, 0.0f, 0.0f);
		glVertex3f(x+0, y+h, z-r[n]);
		glVertex3f(x+0, y-h, z-r[n]);
		glVertex3f(x+r[n], y-h, z+0);
		glVertex3f(x+r[n], y+h, z+0);
		glColor3f(0.9f, 0.0f, 0.0f);

		//4시 벽
		glNormal3f(1, 0, 1);
		glColor3f(-0.0f, 0.0f, 1.0f);
		glVertex3f(x+r[n], y+h, z+0);
		glVertex3f(x+r[n], y-h, z+0);
		glVertex3f(x+0, y-h, z+r[n]);
		glVertex3f(x+0, y+h, z+r[n]);
		
		//8시 벽
		glNormal3f(-1, 0, 1);
		glColor3f(-0.0f, 0.0f, 1.0f);
		glVertex3f(x-r[n], y+h, z+0);
		glVertex3f(x-r[n], y-h, z+0);
		glVertex3f(x+0, y-h, z+r[n]);
		glVertex3f(x+0, y+h, z+r[n]);

		//11시벽
		glNormal3f(-1, 0, -1);
		glColor3f(-0.0f, 0.0f, 1.0f);
		glVertex3f(x+0, y+h, z-r[n]);
		glVertex3f(x+0, y-h, z-r[n]);
		glVertex3f(x-r[n], y-h, z+0);
		glVertex3f(x-r[n], y+h, z+0);

	glEnd();
}

//시점 세팅
void CameraSetting(void){
	// 화면을 제대로 바라보기 위해 카메라를 회전 후 이동
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	//기준시점
	gluLookAt(
		0.0f, 25.0f, 30.0f,
		0.0f, 10.0f, 0.0f,
		0.0f, 1.0f, 0.0f);

	glTranslatef(0.0f, CamDiffY, CamDiffZ); //시점 확대축소
	glRotatef(angle, 0.0f, 1.0f, 0.0f); //시점 회전
}

//그리기
void Rendering(){
	
	// 화면 버퍼 클리어
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	CameraSetting();

	RenderPlane();
	
	for(int n=0; n<2; n++){
		glPushMatrix();
		glTranslatef(boxX[n], boxY[n], boxZ[n]);


		//충돌 후 각도 복귀
		if(boxKE[n] < 0){
			glTranslatef( -r[n], 0.0f, 0.0f);
		}

		//변화
		glRotatef(angleY[n], 0.0f, 1.0f, 0.0f);
		glRotatef(angleX[n], 1.0f, 0.0f, 0.0f);
		glRotatef(angleZ[n], 0.0f, 0.0f, 1.0f);
	
		//충돌 후 각도 복귀
		if(boxKE[n] < 0){
			glTranslatef( r[n], 0.0f, 0.0f);
		}

		glColor3f(0.9f, 0.0f, 0.0f); 
		RenderBox(0.0f, 0.0f, 0.0f, n);
		glPopMatrix();
	}
	
	
	// back 버퍼에 랜더링한 후 swap
	glutSwapBuffers();
}

void Reshape(int w, int h)
{
	// 뷰포트 설정
	glViewport(0, 0, w, h);

	// 원근 투영 사용
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45, (float)w / h, 0.1, 500);

	// 모델뷰 매트릭스 초기화
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void Keyboard(unsigned char key, int x, int y)
{
	//카메라 시점
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

	//충돌
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
		
		case 'q': //리셋
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

//충돌을 체크한다(몇번째 정육면체인지 명시해주어야 한다)
void checkCollision(int n){

	float startAngle = atan(0.70710678118); //중점과 정육면체 모서리가 이루는 각
	float edgeLen = sqrt(r[n]*r[n] + r[n]*r[n] + r[n]*r[n]);

	if(boxSpeed[n] > 0){ //충돌은 내려갈때만 판정
		if( (boxY[n] - edgeLen*sin( startAngle + angleX[n]/180*PI )) < 0){
			collisionPoint[n]=6;
			cout<< "6시충돌" << endl;
		}else if( (boxY[n] - edgeLen*sin( startAngle - angleX[n]/180*PI )) < 0 ){
			collisionPoint[n]=12;
			cout << "12시 충돌" << endl;
		}else if( (boxY[n] - edgeLen*sin( startAngle + angleZ[n]/180*PI )) < 0){
			collisionPoint[n]=3;
			cout<< "3시충돌" << endl;
		}else if( (boxY[n] - edgeLen*sin( startAngle - angleZ[n]/180*PI )) < 0){
			collisionPoint[n]=9;
			cout<< "9시충돌" << endl;
		}
	}
}

void TimerFunc(int value)
{

	checkCollision(0);
	checkCollision(1);

	//int n=0;

	for(int n=0; n<2; n++){
		//0.01초마다 시행
		if(boxKE[n] > 0){

			//하강 시 변화량
			angleX[0] += 0.03;
			angleX[1] -= 0.01;

			angleY[0] += 0.15;
			angleY[1] -= 0.25;

			angleZ[n] += 0.02;

			/*
			boxX += 0.001;
			boxZ += 0.001;*/

			boxSpeed[n] += 0.001f; //스피드가 일정한 비율로 올라가는데 이게 가속도(g)의 역할을 한다
		
			//바닥면과 충돌
			if(collisionPoint[n] != 0 && boxSpeed > 0){ 

				if(boxSpeed[n] > 0.02){ //다시 튀어오르는 경우
					cout<< "[" << n << "] 쿵! 속도:" << boxSpeed[n] << endl;
					boxSpeed[n] = boxSpeed[n] * 0.5;
					boxSpeed[n] = boxSpeed[n] * (-1);
					collisionPoint[n]=0;
				}else{ //끝
					cout<< "[" << n << "] 끝쿵! 속도:" << boxSpeed[n] << endl;
					boxKE[n] = 0;
					boxSpeed[n] = 0;
					cout << "[" << n << "] 정지" << endl;
				}
			}
		}else{ //정지상황에서 평평함을 복귀(kE = 0) 
			if(angleX[n] > 1){
				//cout << "복구" << endl;
				angleX[n] -= 0.4f*time*time*time;
				boxY[n] -= 0.01f;
				boxX[n] += 0.02f;
				boxZ[n] += 0.02f;

			}else{
				//cout << "복구완료" << endl;
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
	glutTimerFunc(1, TimerFunc, 1); //타이머

	glutKeyboardFunc(Keyboard);  // 키보드 입력 처리 Callback 함수 등록
	glutDisplayFunc(Rendering);  // 변환된 값에 따른 Rendering Callback 함수 등록
	glutReshapeFunc(Reshape);    // 윈도우 창 크기가 바뀌었을때 호출되는 Callback 함수 등록
	glutMouseFunc(Mouse);        // 마우스 클릭 처리 Callback 함수 등록
	glutMotionFunc(Motion);      // 마우스 움직임 처리 Callback 함수 등록

	glutMainLoop(); // 등록된 callback 함수를 반복하여 호출
}



int main(int argc, char** argv)
{
	Initialize(argc, argv);			  // 윈도우 생성, 배경색 설정

	MeshLoad();       //To Do
	ComputeNormal();  //To Do
	EventHandlingAndLoop();      // Event Handling 및 Loop

	// 에러 없이 끝났을 경우 0을 리턴함
	return 0;
}
