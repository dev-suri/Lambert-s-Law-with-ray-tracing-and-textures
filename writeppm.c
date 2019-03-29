#
define M 640 // pixel width and length of image
# define N 480# include < math.h > #include < stdio.h >
  typedef struct {
    double x;
    double y;
    double z;
  }
triple;
//
triple eye = {
  0.50,
  0.50,
  -1.00
}; // the eye
triple g = {
  0.00,
  1.25,
  -0.50
}; // the light

double dotp(triple t, triple u) {
  return t.x * u.x + t.y * u.y + t.z * u.z;
}

triple diff(triple u, triple v) // t = u - v
{
  triple t = {
    u.x - v.x,
    u.y - v.y,
    u.z - v.z
  };
  return t;
}

typedef struct {
  int r;
  int g;
  int b;
}
color;

typedef struct {
  triple c;
  double r;
  color h;
  color h2;
  int check;
}
sphere;

sphere a[4]; //four spheres
void init() {
  a[0].c.x = 0.50;
  a[0].c.y = -20000.00; // the floor
  a[0].c.z = 0.50;
  a[0].r = 20000.25;
  a[0].h.r = 205; // color is Peru
  a[0].h.g = 133;
  a[0].h.b = 63;
  a[0].h2.r = 120;
  a[0].h2.g = 81;
  a[0].h2.b = 169;
  a[0].check = 1;
  //
  a[1].c.x = 0.50;
  a[1].c.y = 0.50;
  a[1].c.z = 0.50;
  a[1].r = 0.25;
  a[1].h.r = 0; // color is Blue
  a[1].h.g = 0;
  a[1].h.b = 255;
  a[1].check = 0;
  //
  a[2].c.x = 1.00;
  a[2].c.y = 0.50;
  a[2].c.z = 1.00;
  a[2].r = 0.25;
  a[2].h.r = 0; // color is Green
  a[2].h.g = 255;
  a[2].h.b = 0;
  a[2].check = 0;
  //
  a[3].c.x = 0.00;
  a[3].c.y = 0.75;
  a[3].c.z = 1.25;
  a[3].r = 0.50;
  a[3].h.r = 255; // color is Red
  a[3].h.g = 0;
  a[3].h.b = 0;
  a[3].check = 0;
}

//
int isblocked(triple intersect) {
  //returns whether a given ray from light to destination is blocked, and therfore whether a shadow is present
  //1 means shadow, 0 means no shadow
  triple shadow = diff(g, intersect);
  double mag_shadow = sqrt((shadow.x * shadow.x) + (shadow.y * shadow.y) + (shadow.z * shadow.z));
  shadow.x = shadow.x / mag_shadow;
  shadow.y = shadow.y / mag_shadow;
  shadow.z = shadow.z / mag_shadow;
  //

  //
  //quadratic formula to find intersections between rays and spheres, discr refers to discriminant of quadratic formula (root(b^2-4ac))
  double aco = 1.0;
  int asize = sizeof(a) / sizeof(a[0]);
  for (int k = 0; k < asize; k++) {
    triple ray = diff(intersect, a[k].c);
    double b = 2.0 * dotp(shadow, ray);
    double c = dotp(ray, ray) - (a[k].r * a[k].r);
    double discr = (b * b) - (4 * aco * c);
    if (discr > 0) {
      double distance1 = (-b + sqrt(discr)) / 2.0;
      double distance2 = (-b - sqrt(discr)) / 2.0;
      if (distance1 > 0) {
        return 1;
      }
      if (distance2 > 0) {
        return 1;
      }
    }

  }
  return 0;

}

color calculate(double x, double z, sphere s) {
  if (s.check == 0) {
    return s.h;
  }
  if (((int) floor(x * 10) + (int) floor(z * 10)) % 2 == 0) {
    return s.h;
  } else {
    return s.h2;
  }
}

int main() {
  init();
  int rgb[N][M][3]; // red-green-blue for each pixel
  for (int y = 0; y < N; y++) {
    for (int x = 0; x < M; x++) {
      int closest = -1;
      double distance = INFINITY;
      triple pixel = {
        x * 1.0 / M + (1 / (M * 2)),
        (1.0 - (y * (1.0 / N))) + (1 / (N * 2)),
        0
      };
      triple ray = diff(pixel, eye);
      double mag_r = sqrt((ray.x * ray.x) + (ray.y * ray.y) + (ray.z * ray.z));
      ray.x = ray.x / mag_r;
      ray.y = ray.y / mag_r;
      ray.z = ray.z / mag_r;

      double aco = 1.0;
      int asize = sizeof(a) / sizeof(a[0]);
      for (int k = 0; k < asize; k++) {
        triple eyecenter = diff(eye, a[k].c);
        double b = 2.0 * dotp(ray, eyecenter);
        double c = dotp(eyecenter, eyecenter) - (a[k].r * a[k].r);
        double discr = (b * b) - (4 * aco * c);
        if (discr > 0) {
          double distance1 = (-b + sqrt(discr)) / 2.0;
          double distance2 = (-b - sqrt(discr)) / 2.0;
          if (distance1 > 0 && distance1 < distance) {
            distance = distance1;
            closest = k;
          }
          if (distance2 > 0 && distance2 < distance) {
            distance = distance2;
            closest = k;
          }
        }

      }

      if (closest == -1) {
        rgb[y][x][0] = 0;
        rgb[y][x][1] = 0;
        rgb[y][x][2] = 0;

      } else {
        triple intersect = {
          (ray.x * distance) + eye.x,
          (ray.y * distance) + eye.y,
          (ray.z * distance) + eye.z
        };

        triple normal = diff(intersect, a[closest].c);
        double mag_normal = sqrt((normal.x * normal.x) + (normal.y * normal.y) + (normal.z * normal.z));
        normal.x = normal.x / mag_normal;
        normal.y = normal.y / mag_normal;
        normal.z = normal.z / mag_normal;
        triple normalized = {
          intersect.x + 0.001 * normal.x,
          intersect.y + 0.001 * normal.y,
          intersect.z + 0.001 * normal.z
        };
        double ambient = 0.5;
        color result = calculate(intersect.x, intersect.z, a[closest]);

        if (isblocked(normalized)) {
          rgb[y][x][0] = (int)(result.r * ambient);
          rgb[y][x][1] = (int)(result.g * ambient);
          rgb[y][x][2] = (int)(result.b * ambient);

        } else {
          triple shadow = diff(g, intersect);
          double mag_shadow = sqrt((shadow.x * shadow.x) + (shadow.y * shadow.y) + (shadow.z * shadow.z));
          shadow.x = shadow.x / mag_shadow;
          shadow.y = shadow.y / mag_shadow;
          shadow.z = shadow.z / mag_shadow;
          double info = dotp(shadow, normal);
          if (info < 0) {
            info = 0;
          }

          double scale = info * (1 - ambient) + ambient;
          rgb[y][x][0] = result.r * scale;
          rgb[y][x][1] = result.g * scale;
          rgb[y][x][2] = result.b * scale;

        }
      }

    }
  }
  //
  int x = 0;
  int y = 0;
  FILE * fout;
  fout = fopen("shadow.ppm", "w");
  //
  fprintf(fout, "P3\n");
  fprintf(fout, "%d %d\n", M, N);
  fprintf(fout, "255\n");
  //
  for (y = 0; y < N; y++) {
    for (x = 0; x < M; x++) {
      fprintf(fout, "%d %d %d\n",
        rgb[y][x][0], rgb[y][x][1], rgb[y][x][2]);
    }
  }

  fclose(fout);
  return 0;
}

//
// end of file
//
