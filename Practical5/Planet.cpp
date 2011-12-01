//
//  Planet.cpp
//  Practical4

#include <math.h>

#include "Planet.h"
#include "Material.h"
#include "Glut.h"

// For controlling the speed of the bodies 
float Body::speed_multiplier = 1;

// constructors
Body::Body(MaterialList* materials, float radius, float offset, float speed, float angle):
	materials(materials), radius(radius), offset(offset), speed(speed), angle(angle), orbit(false)
{
	x = cosf((angle) /180.f * (float)M_PI ) * (radius+offset +0.05f);
	z = sinf((angle) /180.f * (float)M_PI ) * (radius+offset+ 0.05f);
}

Planet::Planet(MaterialList* materials, float radius, float offset, float speed, float angle, Body* moon):
	Body(materials,radius,offset,speed,angle),moon(moon),ring(0)
{}

Star::Star(MaterialList* materials, const GLfloat *emission, float radius,bool show_emission, float offset, float speed, float angle):
	Body(materials,radius,offset,speed,angle),emission(emission), show_emission(show_emission)
{}

// Drawing functions
void Body::draw() const
{
	glPushMatrix();
	glColor3fv(materials->current_colour());
	if (orbit) draw_orbit();
	glRotatef(angle, 0.0f, 1.0f, 0.0f);
	glTranslatef(offset, 0, 0);
	draw_material();
	glutSolidSphere(radius, 60, 60);
	draw_other();
	
	glPopMatrix();
}

void Body::draw_material() const
{
	materials->current()->draw();
}

void Star::draw_material() const
{
	if (show_emission) {
		materials->current()->draw_with_emission(emission);
	} else {
		materials->current()->draw();
	}
}

void Body::draw_other() const {}

void Planet::draw_other() const
{
	if (moon){
		moon->draw();
	}
	if (ring){
		ring->draw();
	}
}


static const GLfloat orbit_emission[] = {0.0F, 0.4F, 0.6F, 0.3F};

void Body::draw_orbit() const
{
	Material::blue()->draw_with_emission(orbit_emission);
	glColor3fv(Colour::white());
	glBegin(GL_LINE_LOOP);
	int slices = 40;
	for (int i = 0; i < slices; ++i) {
		const float value = 2 * (float)M_PI/(float)slices * (float)i;
		glVertex3f(offset * sinf(value), 0.0f, offset * cosf(value));
	}
	glEnd();

	// Old method, look better, but orbits are too thick.
//	glPushMatrix();
//		glRotatef(90, 1, 0, 0);
//		glutSolidTorus(0.01, offset, 100, 100);
//	glPopMatrix();

}

// State changing methods

void Planet::set_ring(Ring* new_ring)
{
	ring = new_ring;
}

void Planet::update()
{
	Body::update();
	if (moon) moon->update();
}

void Body::update()
{
	angle+=speed * speed_multiplier;
	x = cosf((angle) /180.f * (float)M_PI ) * (radius+offset +0.05f);
	z = sinf((angle) /180.f * (float)M_PI ) * (radius+offset+ 0.05f);
}

void Body::next_material()
{
	materials->next();
}

void Body::toggle_draw_orbit()
{
	orbit = !orbit;
}

void Star::toggle_emission()
{
	show_emission = !show_emission;
}


Ring::Ring(float inner_ring, float outer_ring, float rotate_angle):
	inner_ring(inner_ring), outer_ring(outer_ring), rotate_angle(rotate_angle)
{}


void Ring::draw() const
{
	glPushMatrix();
	glRotatef(rotate_angle, 1.0f, 0.0f, 0.0f);
	glutSolidTorus(inner_ring, outer_ring, 16, 20);
	glPopMatrix();
}