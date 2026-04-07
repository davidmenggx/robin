#pragma once

/*
We use these coefficients a-f to capture an affine transformation
x' = ax + by + c
y' = dx + ey + f
(this is just a matrix multiplication and vector addition)
*/
struct Affine {
	float a_{};
	float b_{};
	float c_{};
	float d_{};
	float e_{};
	float f_{};
};
