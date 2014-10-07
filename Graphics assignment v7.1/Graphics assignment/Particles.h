#include<vector>

const float DEG2RAD = 3.14159/180;


float* billboard()
{
	float modelview[16];
	int i,j;
	// save the current modelview matrix
	glPushMatrix();
	// get the current modelview matrix
	glGetFloatv(GL_MODELVIEW_MATRIX , modelview);
	// undo all rotations
	for( i=0; i<3; i++ ) 
		for( j=0; j<3; j++ ) {
			if ( i==j )
				modelview[i*4+j] = 1.0;
			else
				modelview[i*4+j] = 0.0;
		}
		float* view = new float[16];
		for(int i = 0; i<16;i++)
		{
			*(view+i) = modelview[i];
		}
		return view;
}

void drawCircle(float radius)
{
	glBegin(GL_TRIANGLE_FAN);
	glVertex3f(0,0,0);
	for (int i=0; i <= 360; i+=30)
	{
		float degInRad = i*DEG2RAD;
		glVertex3f(cos(degInRad)*radius,sin(degInRad)*radius,0);

	}

	glEnd();
}

struct particle
{
	float x;
	float y;
	float z;
	float red;
	float green;
	float blue;
	float size;
	float alpha;
	int time;
};
class particles
{
protected:
	std::vector<particle> particle_list;
public:
	void draw();
	void add(float x, float y, float z, float red, float green, float blue, float size,int time);
};
void particles::add(float x, float y, float z, float red, float green, float blue, float size,int time)
{
	particle new_particle;
	new_particle.x = x;
	new_particle.y = y;
	new_particle.z = z;
	new_particle.red = red;
	new_particle.green = green;
	new_particle.blue = blue;
	new_particle.size = size;
	new_particle.time = time;
	new_particle.alpha= 1;
	particle_list.push_back(new_particle);
}
void particles::draw()
{
	glPushMatrix();
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_TEXTURE_2D);
	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_BLEND); 
	glBlendFunc(GL_SRC_ALPHA,GL_ONE); 

	float* view;


	for(int i = 0; i<particle_list.size();i++)
	{
		if(particle_list[i].alpha>0)
		{
			glColor4f(particle_list[i].red,particle_list[i].green,particle_list[i].blue,particle_list[i].alpha);
			glTranslatef(particle_list[i].x,particle_list[i].y,particle_list[i].z);
			view = billboard();
			glLoadMatrixf(view);
			drawCircle(particle_list[i].size);
			glPopMatrix();
			delete []view;
			particle_list[i].alpha-= 0.01;
		}

		glColor3f(1,1,1);
		
	}



	glDisable(GL_COLOR_MATERIAL);
	glEnable(GL_TEXTURE_2D);
	glDisable(GL_BLEND); 
	glPopMatrix();
}