/* Simplex.cpp
 *
 * Copyright 2007 Eliot Eshelman
 * battlestartux@6by9.net
 *
 *
 *  This file is part of Battlestar Tux.
 *
 *  Battlestar Tux is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; version 2 of the License.
 *
 *  Battlestar Tux is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with Battlestar Tux; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 *
 */


#include <math.h>
#include <SparkPlug/Simplex.h>


namespace SparkPlug
{

int FastFloor( const float );

float Dot( const int*, const float, const float );
float Dot( const int*, const float, const float, const float );
float Dot( const int*, const float, const float, const float, const float );


// The gradients are the midpoints of the vertices of a cube.
static const int Grad3[12][3] = {
	{1,1,0}, {-1,1,0}, {1,-1,0}, {-1,-1,0},
	{1,0,1}, {-1,0,1}, {1,0,-1}, {-1,0,-1},
	{0,1,1}, {0,-1,1}, {0,1,-1}, {0,-1,-1}
};


// The gradients are the midpoints of the vertices of a hypercube.
static const int Grad4[32][4]= {
	{0,1,1,1},  {0,1,1,-1},  {0,1,-1,1},  {0,1,-1,-1},
	{0,-1,1,1}, {0,-1,1,-1}, {0,-1,-1,1}, {0,-1,-1,-1},
	{1,0,1,1},  {1,0,1,-1},  {1,0,-1,1},  {1,0,-1,-1},
	{-1,0,1,1}, {-1,0,1,-1}, {-1,0,-1,1}, {-1,0,-1,-1},
	{1,1,0,1},  {1,1,0,-1},  {1,-1,0,1},  {1,-1,0,-1},
	{-1,1,0,1}, {-1,1,0,-1}, {-1,-1,0,1}, {-1,-1,0,-1},
	{1,1,1,0},  {1,1,-1,0},  {1,-1,1,0},  {1,-1,-1,0},
	{-1,1,1,0}, {-1,1,-1,0}, {-1,-1,1,0}, {-1,-1,-1,0}
};


// Permutation table.  The same list is repeated twice.
static const int perm[512] = {
	151,160,137,91,90,15,131,13,201,95,96,53,194,233,7,225,140,36,103,30,69,142,
	8,99,37,240,21,10,23,190,6,148,247,120,234,75,0,26,197,62,94,252,219,203,117,
	35,11,32,57,177,33,88,237,149,56,87,174,20,125,136,171,168,68,175,74,165,71,
	134,139,48,27,166,77,146,158,231,83,111,229,122,60,211,133,230,220,105,92,41,
	55,46,245,40,244,102,143,54,65,25,63,161,1,216,80,73,209,76,132,187,208, 89,
	18,169,200,196,135,130,116,188,159,86,164,100,109,198,173,186,3,64,52,217,226,
	250,124,123,5,202,38,147,118,126,255,82,85,212,207,206,59,227,47,16,58,17,182,
	189,28,42,223,183,170,213,119,248,152,2,44,154,163,70,221,153,101,155,167,43,
	172,9,129,22,39,253,19,98,108,110,79,113,224,232,178,185,112,104,218,246,97,
	228,251,34,242,193,238,210,144,12,191,179,162,241,81,51,145,235,249,14,239,
	107,49,192,214,31,181,199,106,157,184,84,204,176,115,121,50,45,127,4,150,254,
	138,236,205,93,222,114,67,29,24,72,243,141,128,195,78,66,215,61,156,180,

	151,160,137,91,90,15,131,13,201,95,96,53,194,233,7,225,140,36,103,30,69,142,
	8,99,37,240,21,10,23,190,6,148,247,120,234,75,0,26,197,62,94,252,219,203,117,
	35,11,32,57,177,33,88,237,149,56,87,174,20,125,136,171,168,68,175,74,165,71,
	134,139,48,27,166,77,146,158,231,83,111,229,122,60,211,133,230,220,105,92,41,
	55,46,245,40,244,102,143,54,65,25,63,161,1,216,80,73,209,76,132,187,208, 89,
	18,169,200,196,135,130,116,188,159,86,164,100,109,198,173,186,3,64,52,217,226,
	250,124,123,5,202,38,147,118,126,255,82,85,212,207,206,59,227,47,16,58,17,182,
	189,28,42,223,183,170,213,119,248,152,2,44,154,163,70,221,153,101,155,167,43,
	172,9,129,22,39,253,19,98,108,110,79,113,224,232,178,185,112,104,218,246,97,
	228,251,34,242,193,238,210,144,12,191,179,162,241,81,51,145,235,249,14,239,
	107,49,192,214,31,181,199,106,157,184,84,204,176,115,121,50,45,127,4,150,254,
	138,236,205,93,222,114,67,29,24,72,243,141,128,195,78,66,215,61,156,180
};


// A lookup table to traverse the Simplex around a given point in 4D.
static const int Simplex[64][4] = {
	{0,1,2,3},{0,1,3,2},{0,0,0,0},{0,2,3,1},{0,0,0,0},{0,0,0,0},{0,0,0,0},{1,2,3,0},
	{0,2,1,3},{0,0,0,0},{0,3,1,2},{0,3,2,1},{0,0,0,0},{0,0,0,0},{0,0,0,0},{1,3,2,0},
	{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},
	{1,2,0,3},{0,0,0,0},{1,3,0,2},{0,0,0,0},{0,0,0,0},{0,0,0,0},{2,3,0,1},{2,3,1,0},
	{1,0,2,3},{1,0,3,2},{0,0,0,0},{0,0,0,0},{0,0,0,0},{2,0,3,1},{0,0,0,0},{2,1,3,0},
	{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},
	{2,0,1,3},{0,0,0,0},{0,0,0,0},{0,0,0,0},{3,0,1,2},{3,0,2,1},{0,0,0,0},{3,1,2,0},
	{2,1,0,3},{0,0,0,0},{0,0,0,0},{0,0,0,0},{3,1,0,2},{0,0,0,0},{3,2,0,1},{3,2,1,0}
};





// 2D Multi-octave Simplex noise.
//
// For each octave, a higher frequency/lower amplitude function will be added to the original.
// The higher the persistence [0-1], the more of each succeeding octave will be added.
float SimplexNoise( const int octaves, const float persistence, const float scale, const vec2f pos )
{
	float total = 0;
	float frequency = scale;
	float amplitude = 1;

	// We have to keep track of the largest possible amplitude,
	// because each octave adds more, and we need a value in [-1, 1].
	float maxAmplitude = 0;

	for( int i=0; i < octaves; i++ ) {
		total += SimplexRawNoise( pos * frequency ) * amplitude;

		frequency *= 2;
		maxAmplitude += amplitude;
		amplitude *= persistence;
	}

	return total / maxAmplitude;
}


// 3D Multi-octave Simplex noise.
//
// For each octave, a higher frequency/lower amplitude function will be added to the original.
// The higher the persistence [0-1], the more of each succeeding octave will be added.
float SimplexNoise( const int octaves, const float persistence, const float scale, const vec3f pos )
{
	float total = 0;
	float frequency = scale;
	float amplitude = 1;

	// We have to keep track of the largest possible amplitude,
	// because each octave adds more, and we need a value in [-1, 1].
	float maxAmplitude = 0;

	for( int i=0; i < octaves; i++ ) {
		total += SimplexRawNoise( pos * frequency ) * amplitude;

		frequency *= 2;
		maxAmplitude += amplitude;
		amplitude *= persistence;
	}

	return total / maxAmplitude;
}


// 4D Multi-octave Simplex noise.
//
// For each octave, a higher frequency/lower amplitude function will be added to the original.
// The higher the persistence [0-1], the more of each succeeding octave will be added.
float SimplexNoise( const int octaves, const float persistence, const float scale, const vec4f pos ) {
	float total = 0;
	float frequency = scale;
	float amplitude = 1;

	// We have to keep track of the largest possible amplitude,
	// because each octave adds more, and we need a value in [-1, 1].
	float maxAmplitude = 0;

	for( int i=0; i < octaves; i++ ) {
		total += SimplexRawNoise( pos * frequency ) * amplitude;

		frequency *= 2;
		maxAmplitude += amplitude;
		amplitude *= persistence;
	}

	return total / maxAmplitude;
}


// 2D raw Simplex noise
float SimplexRawNoise( const vec2f pos )
{
	// Noise contributions from the three corners
	float n0, n1, n2;

	// Skew the input space to determine which Simplex cell we're in
	float F2 = 0.5 * (sqrtf(3.0) - 1.0);
	// Hairy factor for 2D
	float s = (pos.x + pos.y) * F2;
	int i = FastFloor( pos.x + s );
	int j = FastFloor( pos.y + s );

	float G2 = (3.0 - sqrtf(3.0)) / 6.0;
	float t = (i + j) * G2;
	// Unskew the cell origin back to (x,y) space
	float X0 = i-t;
	float Y0 = j-t;
	// The x,y distances from the cell origin
	float x0 = pos.x-X0;
	float y0 = pos.y-Y0;

	// For the 2D case, the Simplex shape is an equilateral triangle.
	// Determine which Simplex we are in.
	int i1, j1; // Offsets for second (middle) corner of Simplex in (i,j) coords
	if(x0>y0) {i1=1; j1=0;} // lower triangle, XY order: (0,0)->(1,0)->(1,1)
	else {i1=0; j1=1;} // upper triangle, YX order: (0,0)->(0,1)->(1,1)

	// A step of (1,0) in (i,j) means a step of (1-c,-c) in (x,y), and
	// a step of (0,1) in (i,j) means a step of (-c,1-c) in (x,y), where
	// c = (3-sqrt(3))/6
	float x1 = x0 - i1 + G2; // Offsets for middle corner in (x,y) unskewed coords
	float y1 = y0 - j1 + G2;
	float x2 = x0 - 1.0 + 2.0 * G2; // Offsets for last corner in (x,y) unskewed coords
	float y2 = y0 - 1.0 + 2.0 * G2;

	// Work out the hashed Gradient indices of the three Simplex corners
	int ii = i & 255;
	int jj = j & 255;
	int gi0 = perm[ii+perm[jj]] % 12;
	int gi1 = perm[ii+i1+perm[jj+j1]] % 12;
	int gi2 = perm[ii+1+perm[jj+1]] % 12;

	// Calculate the contribution from the three corners
	float t0 = 0.5 - x0*x0-y0*y0;
	if(t0<0) n0 = 0.0;
	else {
	t0 *= t0;
	n0 = t0 * t0 * Dot(Grad3[gi0], x0, y0); // (x,y) of Grad3 used for 2D Gradient
	}

	float t1 = 0.5 - x1*x1-y1*y1;
	if(t1<0) n1 = 0.0;
	else {
	t1 *= t1;
	n1 = t1 * t1 * Dot(Grad3[gi1], x1, y1);
	}

	float t2 = 0.5 - x2*x2-y2*y2;
	if(t2<0) n2 = 0.0;
	else {
	t2 *= t2;
	n2 = t2 * t2 * Dot(Grad3[gi2], x2, y2);
	}

	// Add contributions from each corner to get the final noise value.
	// The result is scaled to return values in the interval [-1,1].
	return 70.0 * (n0 + n1 + n2);
}


// 3D raw Simplex noise
float SimplexRawNoise( const vec3f pos ) {
	float n0, n1, n2, n3; // Noise contributions from the four corners

	// Skew the input space to determine which Simplex cell we're in
	float F3 = 1.0/3.0;
	float s = (pos.x+pos.y+pos.z)*F3; // Very nice and simple skew factor for 3D
	int i = FastFloor(pos.x+s);
	int j = FastFloor(pos.y+s);
	int k = FastFloor(pos.z+s);

	float G3 = 1.0/6.0; // Very nice and simple unskew factor, too
	float t = (i+j+k)*G3;
	float X0 = i-t; // Unskew the cell origin back to (x,y,z) space
	float Y0 = j-t;
	float Z0 = k-t;
	float x0 = pos.x-X0; // The x,y,z distances from the cell origin
	float y0 = pos.y-Y0;
	float z0 = pos.z-Z0;

	// For the 3D case, the Simplex shape is a slightly irregular tetrahedron.
	// Determine which Simplex we are in.
	int i1, j1, k1; // Offsets for second corner of Simplex in (i,j,k) coords
	int i2, j2, k2; // Offsets for third corner of Simplex in (i,j,k) coords

	if(x0>=y0) {
	if(y0>=z0)
	{ i1=1; j1=0; k1=0; i2=1; j2=1; k2=0; } // X Y Z order
	else if(x0>=z0) { i1=1; j1=0; k1=0; i2=1; j2=0; k2=1; } // X Z Y order
	else { i1=0; j1=0; k1=1; i2=1; j2=0; k2=1; } // Z X Y order
	}
	else { // x0<y0
	if(y0<z0) { i1=0; j1=0; k1=1; i2=0; j2=1; k2=1; } // Z Y X order
	else if(x0<z0) { i1=0; j1=1; k1=0; i2=0; j2=1; k2=1; } // Y Z X order
	else { i1=0; j1=1; k1=0; i2=1; j2=1; k2=0; } // Y X Z order
	}

	// A step of (1,0,0) in (i,j,k) means a step of (1-c,-c,-c) in (x,y,z),
	// a step of (0,1,0) in (i,j,k) means a step of (-c,1-c,-c) in (x,y,z), and
	// a step of (0,0,1) in (i,j,k) means a step of (-c,-c,1-c) in (x,y,z), where
	// c = 1/6.
	float x1 = x0 - i1 + G3; // Offsets for second corner in (x,y,z) coords
	float y1 = y0 - j1 + G3;
	float z1 = z0 - k1 + G3;
	float x2 = x0 - i2 + 2.0*G3; // Offsets for third corner in (x,y,z) coords
	float y2 = y0 - j2 + 2.0*G3;
	float z2 = z0 - k2 + 2.0*G3;
	float x3 = x0 - 1.0 + 3.0*G3; // Offsets for last corner in (x,y,z) coords
	float y3 = y0 - 1.0 + 3.0*G3;
	float z3 = z0 - 1.0 + 3.0*G3;

	// Work out the hashed Gradient indices of the four Simplex corners
	int ii = i & 255;
	int jj = j & 255;
	int kk = k & 255;
	int gi0 = perm[ii+perm[jj+perm[kk]]] % 12;
	int gi1 = perm[ii+i1+perm[jj+j1+perm[kk+k1]]] % 12;
	int gi2 = perm[ii+i2+perm[jj+j2+perm[kk+k2]]] % 12;
	int gi3 = perm[ii+1+perm[jj+1+perm[kk+1]]] % 12;

	// Calculate the contribution from the four corners
	float t0 = 0.6 - x0*x0 - y0*y0 - z0*z0;
	if(t0<0) n0 = 0.0;
	else {
	t0 *= t0;
	n0 = t0 * t0 * Dot(Grad3[gi0], x0, y0, z0);
	}

	float t1 = 0.6 - x1*x1 - y1*y1 - z1*z1;
	if(t1<0) n1 = 0.0;
	else {
	t1 *= t1;
	n1 = t1 * t1 * Dot(Grad3[gi1], x1, y1, z1);
	}

	float t2 = 0.6 - x2*x2 - y2*y2 - z2*z2;
	if(t2<0) n2 = 0.0;
	else {
	t2 *= t2;
	n2 = t2 * t2 * Dot(Grad3[gi2], x2, y2, z2);
	}

	float t3 = 0.6 - x3*x3 - y3*y3 - z3*z3;
	if(t3<0) n3 = 0.0;
	else {
	t3 *= t3;
	n3 = t3 * t3 * Dot(Grad3[gi3], x3, y3, z3);
	}

	// Add contributions from each corner to get the final noise value.
	// The result is scaled to stay just inside [-1,1]
	return 32.0*(n0 + n1 + n2 + n3);
}


// 4D raw Simplex noise
float SimplexRawNoise( const vec4f pos ) {
	// The skewing and unskewing factors are hairy again for the 4D case
	float F4 = (sqrtf(5.0)-1.0)/4.0;
	float G4 = (5.0-sqrtf(5.0))/20.0;
	float n0, n1, n2, n3, n4; // Noise contributions from the five corners

	// Skew the (x,y,z,w) space to determine which cell of 24 simplices we're in
	float s = (pos.x + pos.y + pos.z + pos.w) * F4; // Factor for 4D skewing
	vec4i i = (pos + s).floor();
	float t = i.sum() * G4; // Factor for 4D unskewing
	vec4f Pos0 = i - t; // Unskew the cell origin back to (x,y,z,w) space
	vec4f pos0 = pos - Pos0; // The x,y,z,w distances from the cell origin

	// For the 4D case, the Simplex is a 4D shape I won't even try to describe.
	// To find out which of the 24 possible simplices we're in, we need to
	// determine the magnitude ordering of x0, y0, z0 and w0.
	// The method below is a good way of finding the ordering of x,y,z,w and
	// then find the correct traversal order for the Simplex we're in.
	// First, six pair-wise comparisons are performed between each possible pair
	// of the four coordinates, and the results are used to add up binary bits
	// for an integer index.
	int c1 = (pos0.x > pos0.y) ? 32 : 0;
	int c2 = (pos0.x > pos0.z) ? 16 : 0;
	int c3 = (pos0.y > pos0.z) ? 8 : 0;
	int c4 = (pos0.x > pos0.w) ? 4 : 0;
	int c5 = (pos0.y > pos0.w) ? 2 : 0;
	int c6 = (pos0.z > pos0.w) ? 1 : 0;
	int c = c1 + c2 + c3 + c4 + c5 + c6;

	vec4i i1; // The integer offsets for the second Simplex corner
	vec4i i2; // The integer offsets for the third Simplex corner
	vec4i i3; // The integer offsets for the fourth Simplex corner

	// Simplex[c] is a 4-vector with the numbers 0, 1, 2 and 3 in some order.
	// Many values of c will never occur, since e.g. x>y>z>w makes x<z, y<w and x<w
	// impossible. Only the 24 indices which have non-zero entries make any sense.
	// We use a thresholding to set the coordinates in turn from the largest magnitude.
	// The number 3 in the "Simplex" array is at the position of the largest coordinate.
	i1.x = Simplex[c][0]>=3 ? 1 : 0;
	i1.y = Simplex[c][1]>=3 ? 1 : 0;
	i1.z = Simplex[c][2]>=3 ? 1 : 0;
	i1.w = Simplex[c][3]>=3 ? 1 : 0;
	// The number 2 in the "Simplex" array is at the second largest coordinate.
	i2.x = Simplex[c][0]>=2 ? 1 : 0;
	i2.y = Simplex[c][1]>=2 ? 1 : 0;
	i2.z = Simplex[c][2]>=2 ? 1 : 0;
	i2.w = Simplex[c][3]>=2 ? 1 : 0;
	// The number 1 in the "Simplex" array is at the second smallest coordinate.
	i3.x = Simplex[c][0]>=1 ? 1 : 0;
	i3.y = Simplex[c][1]>=1 ? 1 : 0;
	i3.z = Simplex[c][2]>=1 ? 1 : 0;
	i3.w = Simplex[c][3]>=1 ? 1 : 0;
	// The fifth corner has all coordinate offsets = 1, so no need to look that up.

	vec4f pos1 = pos0 - i1 + G4; // Offsets for second corner in (x,y,z,w) coords
	vec4f pos2 = pos0 - i2 + 2.0*G4; // Offsets for third corner in (x,y,z,w) coords
	vec4f pos3 = pos0 - i3 + 3.0*G4; // Offsets for fourth corner in (x,y,z,w) coords
	vec4f pos4 = pos0 - 1.0 + 4.0*G4; // Offsets for last corner in (x,y,z,w) coords

	// Work out the hashed Gradient indices of the five Simplex corners
	vec4i ii(
		i.x & 255,
		i.y & 255,
		i.z & 255,
		i.w & 255
	);
	int gi0 = perm[ii.x+     perm[ii.y+     perm[ii.z+     perm[ii.w     ]]]] % 32;
	int gi1 = perm[ii.x+i1.x+perm[ii.y+i1.y+perm[ii.z+i1.z+perm[ii.w+i1.w]]]] % 32;
	int gi2 = perm[ii.x+i2.x+perm[ii.y+i2.y+perm[ii.z+i2.z+perm[ii.w+i2.w]]]] % 32;
	int gi3 = perm[ii.x+i3.x+perm[ii.y+i3.y+perm[ii.z+i3.z+perm[ii.w+i3.w]]]] % 32;
	int gi4 = perm[ii.x+   1+perm[ii.y+   1+perm[ii.z+   1+perm[ii.w+   1]]]] % 32;

	// Calculate the contribution from the five corners
	float t0 = 0.6 - pos0.x*pos0.x - pos0.y*pos0.y - pos0.z*pos0.z - pos0.w*pos0.w;
	if(t0 < 0)
	{
		n0 = 0.0;
	}
	else
	{
		t0 *= t0;
		n0 = t0 * t0 * Dot(Grad4[gi0], pos0.x, pos0.y, pos0.z, pos0.w);
	}

	float t1 = 0.6 - pos1.x*pos1.x - pos1.y*pos1.y - pos1.z*pos1.z - pos1.w*pos1.w;
	if(t1 < 0)
	{
		n1 = 0.0;
	}
	else
	{
		t1 *= t1;
		n1 = t1 * t1 * Dot(Grad4[gi1], pos1.x, pos1.y, pos1.z, pos1.w);
	}
	
	float t2 = 0.6 - pos2.x*pos2.x - pos2.y*pos2.y - pos2.z*pos2.z - pos2.w*pos2.w;
	if(t2 < 0)
	{
		n2 = 0.0;
	}
	else
	{
		t2 *= t2;
		n2 = t2 * t2 * Dot(Grad4[gi2], pos2.x, pos2.y, pos2.z, pos2.w);
	}
	
	float t3 = 0.6 - pos3.x*pos3.x - pos3.y*pos3.y - pos3.z*pos3.z - pos3.w*pos3.w;
	if(t3 < 0)
	{
		n3 = 0.0;
	}
	else
	{
		t3 *= t3;
		n3 = t3 * t3 * Dot(Grad4[gi3], pos3.x, pos3.y, pos3.z, pos3.w);
	}

	float t4 = 0.6 - pos4.x*pos4.x - pos4.y*pos4.y - pos4.z*pos4.z - pos4.w*pos4.w;
	if(t4 < 0)
	{
		n4 = 0.0;
	}
	else
	{
		t4 *= t4;
		n4 = t4 * t4 * Dot(Grad4[gi4], pos4.x, pos4.y, pos4.z, pos4.w);
	}

	// Sum up and scale the result to cover the range [-1,1]
	return 27.0 * (n0 + n1 + n2 + n3 + n4);
}


int FastFloor( const float x ) { return x > 0 ? (int) x : (int) x - 1; }

float Dot( const int* g, const float x, const float y ) { return g[0]*x + g[1]*y; }
float Dot( const int* g, const float x, const float y, const float z ) { return g[0]*x + g[1]*y + g[2]*z; }
float Dot( const int* g, const float x, const float y, const float z, const float w ) { return g[0]*x + g[1]*y + g[2]*z + g[3]*w; }


}
