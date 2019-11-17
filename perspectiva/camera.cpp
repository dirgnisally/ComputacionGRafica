#include "camera.h"

camera::camera( float positionX, float positionY, float positionZ ){
    int ierr = 0;
    this->camViewX = positionX;
    this->camViewY = positionY;
    this->camViewZ = positionZ;
    // looks towards
    this->camAtX = 0.0f;
    this->camAtY = 0.0f;
    this->camAtZ = 0.0f;
    this->camUpX = 0.0f;
    this->camUpY = 1.0f;
    this->camUpZ = 0.0f;
    this->camAperture = 60.0f * DEGREE_TO_RAD;
    this->camNear = 0.5f;
    this->camFar = 200.0f;
    this->camProjection = CAM_CONIC;
    this->aspectRatio = 1.0f;
    SetDependentParametersCamera();
}

camera::camera(const camera& orig){

}

camera::~camera(){

}

void camera::SetDependentParametersCamera (){
    // camera i j k vectors in world coordinates
    // camIX, camIY, camIZ;
    // camJX, camJY, camJZ;
    // camKX, camKY, camKZ;

    float ix, iy, iz;
    float jx, jy, jz;
    float kx, ky, kz;
    float atX, atY, atZ;
    float upX, upY, upZ;
    float viewX, viewY, viewZ;
    int ierr = 0;

    viewX = this->camViewX;
    viewY = this->camViewY;
    viewZ = this->camViewZ;
    atX = this->camAtX;
    atY = this->camAtY;
    atZ = this->camAtZ;
    upX = 0.0f;
    upY = 1.0f;
    upZ = 0.0f;

    // i, j, k, up must be unit vectors
    // k = normalizar( AV )
    // i = normalizar( up ^ k )
    // j = k ^ i
    UnitVectorPP( &ierr, &kx, &ky, &kz, atX, atY, atZ, viewX, viewY, viewZ );
    UnitVectorVV( &ierr, &ix, &iy, &iz, upX, upY, upZ, kx, ky, kz );
    UnitVectorVV( &ierr, &jx, &jy, &jz, kx, ky, kz, ix, iy, iz );

    this->camKX = kx;
    this->camKY = ky;
    this->camKZ = kz;
    this->camIX = ix;
    this->camIY = iy;
    this->camIZ = iz;
    this->camJX = jx;
    this->camJY = jy;
    this->camJZ = jz;
    this->camUpX = jx;
    this->camUpY = jy;
    this->camUpZ = jz;
}

void camera::SetGLCamera(){
    float fovy;

    glMatrixMode( GL_PROJECTION );
    glLoadIdentity( );

    if( this->camProjection == CAM_CONIC ){
        fovy = this->camAperture*RAD_TO_DEGREE;
        gluPerspective(fovy, this->aspectRatio, this->camNear, this->camFar );
    }
    else { // CAM_PARALLEL
        glOrtho(this->x1, this->x2, this->y1, this->y2, this->z1,this->z2);
    }
    gluLookAt(this->camViewX, this->camViewY, this->camViewZ,
                this->camAtX, this->camAtY, this->camAtZ,
                this->camUpX, this->camUpY, this->camUpZ);
    glMatrixMode( GL_MODELVIEW ); //* GL_MODELVIEW *
}

void camera::SetGLAspectRatioCamera(){
    GLint viewport[4];

    glGetIntegerv( GL_VIEWPORT, viewport );
    if( viewport[3] > 0 )
        this->aspectRatio = (float) viewport[2] / (float) viewport[3];// width/height
    else
        this->aspectRatio = 1.0f;
    SetDependentParametersCamera();
}

void camera::SetCamera( float viewX, float viewY, float viewZ,
                               float atX, float atY, float atZ,
                               float upX, float upY, float upZ ){
    this->camViewX = viewX;
    this->camViewY = viewY;
    this->camViewZ = viewZ;
    this->camAtX = atX;
    this->camAtY = atY;
    this->camAtZ = atZ;
    this->camUpX = upX;
    this->camUpY = upY;
    this->camUpZ = upZ;

    SetDependentParametersCamera();
}

void camera::AvanceFreeCamera( float step){
    float vaX, vaY, vaZ;

    vaX= step * this->camKX;
    vaY= step * this->camKY;
    vaZ= step * this->camKZ;

    // Set V & A

    this->camViewX = this->camViewX + vaX;
    this->camViewY = this->camViewY + vaY;
    this->camViewZ = this->camViewZ + vaZ;
    this->camAtX = this->camAtX + vaX;
    this->camAtY = this->camAtY + vaY;
    this->camAtZ = this->camAtZ + vaZ;
    SetDependentParametersCamera();
}

void camera::YawCamera( float angle) {
    float vIn[3];

    vIn[0] = this->camAtX - this->camViewX;
    vIn[1] = this->camAtY - this->camViewY;
    vIn[2] = this->camAtZ - this->camViewZ;

    VectorRotY(vIn, angle);

    this->camAtX = this->camViewX + vIn[0];
    this->camAtY = this->camViewY + vIn[1];
    this->camAtZ = this->camViewZ + vIn[2];

    SetDependentParametersCamera();
}

void camera::Rotar_Longitud(float inc) {
    float vIn[3];

    vIn[0] = this->camViewX - this->camAtX;
    vIn[1] = this->camViewY - this->camAtY;
    vIn[2] = this->camViewZ - this->camAtZ;

    VectorRotY(vIn, inc);

    this->camViewX = this->camAtX + vIn[0];
    this->camViewZ = this->camAtZ + vIn[2];

    SetDependentParametersCamera();
}

void camera::Rotar_Latitud( float inc) {
    float vIn[3];

    vIn[0] = this->camViewX - this->camAtX;
    vIn[1] = this->camViewY - this->camAtY;
    vIn[2] = this->camViewZ - this->camAtZ;

    VectorRotXZ(vIn, inc, true);

    this->camViewX = this->camAtX + vIn[0];
    this->camViewY = this->camAtY + vIn[1];
    this->camViewZ = this->camAtZ + vIn[2];

    SetDependentParametersCamera();
}

void camera::PitchCamera(float angle){
    float vIn[3];

    vIn[0]= this->camAtX - this->camViewX;
    vIn[1]= this->camAtY - this->camViewY;
    vIn[2]= this->camAtZ - this->camViewZ;

    VectorRotXZ( vIn, angle, true );
    this->camAtX = this->camViewX + vIn[0];
    this->camAtY = this->camViewY + vIn[1];
    this->camAtZ = this->camViewZ + vIn[2];

    SetDependentParametersCamera();
}

void camera::PanCamera( float stepX, float stepY ){
    float vaX, vaY, vaZ;

    vaX= stepX * this->camIX + stepY * this->camJX;
    vaY= stepX * this->camIY + stepY * this->camJY;
    vaZ= stepX * this->camIZ + stepY * this->camJZ;

    this->camViewX = this->camViewX + vaX;
    this->camViewY = this->camViewY + vaY;
    this->camViewZ = this->camViewZ + vaZ;
    this->camAtX = this->camAtX + vaX;
    this->camAtY = this->camAtY + vaY;
    this->camAtZ = this->camAtZ + vaZ;

    SetDependentParametersCamera();
}
