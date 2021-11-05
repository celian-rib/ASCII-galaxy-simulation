#ifndef VECTOR_2
#define VECTOR_2

typedef struct {
    float x;
    float y;
} Vector2;

/**
 * @brief Determine the direction between two point and generate the normalized vector of this
 * direction
 *
 * @param a point a
 * @param b point b
 * @return normalized direction (from a to b)
 */
Vector2 normalize_direction(const Vector2 *b, const Vector2 *a);

/**
 * @brief Calculate distance between two coordinates
 *
 * @param ya
 * @param xa
 * @param yb
 * @param xb
 * @return float
 */
float distance(int ya, int xa, int yb, int xb);

/**
 * @brief Calculate distance between two point
 *
 * @param a point a
 * @param b point b
 * @return float
 */
float vector_distance(const Vector2 *a, const Vector2 *b);

#endif
