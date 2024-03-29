
struct vec
{
    union
    {
        struct { float x, y, z; };
        float v[3];
        int i[3];
    };

    vec() {x=y=z=0;}
    vec(float a, float b, float c) : x(a), y(b), z(c) {}
    vec(float *v) : x(v[0]), y(v[1]), z(v[2]) {}

    float &operator[](int i)       { return v[i]; }
    float  operator[](int i) const { return v[i]; }

    bool iszero() const { return x==0 && y==0 && z==0; }

    bool operator==(const vec &o) const { return x == o.x && y == o.y && z == o.z; }
    bool operator!=(const vec &o) const { return x != o.x || y != o.y || z != o.z; }
    vec operator-() const { return vec(-x, -y, -z); }

    vec &mul(float f) { x *= f; y *= f; z *= f; return *this; }
    vec &div(float f) { x /= f; y /= f; z /= f; return *this; }
    vec &add(float f) { x += f; y += f; z += f; return *this; }
    vec &sub(float f) { x -= f; y -= f; z -= f; return *this; }

    vec &add(const vec &o) { x += o.x; y += o.y; z += o.z; return *this; }
    vec &addz(float f) { z += f; return *this; }
    vec &sub(const vec &o) { x -= o.x; y -= o.y; z -= o.z; return *this; }

    float squaredlen() const { return x*x + y*y + z*z; }
    float sqrxy() const { return x*x + y*y; }

    float dot(const vec &o) const { return x*o.x + y*o.y + z*o.z; }
    float dotxy(const vec &o) const { return x*o.x + y*o.y; }

    float magnitude() const { return sqrtf(squaredlen()); }
    vec &normalize() { div(magnitude()); return *this; }

    float dist(const vec &e) const { vec t; return dist(e, t); }
    float dist(const vec &e, vec &t) const { t = *this; t.sub(e); return t.magnitude(); }
    float squareddist(const vec &e) const { vec t = *this; t.sub(e); return t.squaredlen(); }

    float distxy(const vec &e) const { float dx = e.x - x, dy = e.y - y; return sqrtf(dx*dx + dy*dy); }
    float magnitudexy() const { return sqrtf(x*x + y*y); }
    float anglexy() const { return atan2f(x, y) / RAD; }

    bool reject(const vec &o, float max) const { return x>o.x+max || x<o.x-max || y>o.y+max || y<o.y-max; }

    vec &cross(const vec &a, const vec &b) { x = a.y*b.z-a.z*b.y; y = a.z*b.x-a.x*b.z; z = a.x*b.y-a.y*b.x; return *this; }
    float cxy(const vec &a) { return x*a.y-y*a.x; }

    void rotate_around_z(float angle) { *this = vec(cosf(angle)*x-sinf(angle)*y, cosf(angle)*y+sinf(angle)*x, z); }
    void rotate_around_x(float angle) { *this = vec(x, cosf(angle)*y-sinf(angle)*z, cosf(angle)*z+sinf(angle)*y); }
    void rotate_around_y(float angle) { *this = vec(cosf(angle)*x-sinf(angle)*z, y, cosf(angle)*z+sinf(angle)*x); }

    vec &rotate(float angle, const vec &d)
    {
        float c = cosf(angle), s = sinf(angle);
        return rotate(c, s, d);
    }

    vec &rotate(float c, float s, const vec &d)
    {
        *this = vec(x*(d.x*d.x*(1-c)+c) + y*(d.x*d.y*(1-c)-d.z*s) + z*(d.x*d.z*(1-c)+d.y*s),
                    x*(d.y*d.x*(1-c)+d.z*s) + y*(d.y*d.y*(1-c)+c) + z*(d.y*d.z*(1-c)-d.x*s),
                    x*(d.x*d.z*(1-c)-d.y*s) + y*(d.y*d.z*(1-c)+d.x*s) + z*(d.z*d.z*(1-c)+c));
        return *this;
    }

    void orthogonal(const vec &d)
    {
        int i = fabs(d.x) > fabs(d.y) ? (fabs(d.x) > fabs(d.z) ? 0 : 2) : (fabs(d.y) > fabs(d.z) ? 1 : 2);
        v[i] = d[(i+1)%3];
        v[(i+1)%3] = -d[i];
        v[(i+2)%3] = 0;
    }
};


static inline bool htcmp(const vec &x, const vec &y)
{
    return x == y;
}

static inline uint hthash(const vec &k)
{
    return k.i[0]^k.i[1]^k.i[2];
}

struct vec4
{
    union
    {
        struct { float x, y, z, w; };
        float v[4];
    };

    vec4() {}
    explicit vec4(const vec &p, float w = 0) : x(p.x), y(p.y), z(p.z), w(w) {}
    vec4(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {}

    float &operator[](int i)       { return v[i]; }
    float  operator[](int i) const { return v[i]; }
};

struct ivec
{
    union
    {
        struct { int x, y, z; };
        int v[3];
    };

    ivec() {}
    ivec(const vec &v) : x(int(v.x)), y(int(v.y)), z(int(v.z)) {}
    ivec(int a, int b, int c) : x(a), y(b), z(c) {}

    vec tovec() const { return vec(x, y, z); }

    int &operator[](int i)       { return v[i]; }
    int  operator[](int i) const { return v[i]; }

    bool operator==(const ivec &v) const { return x==v.x && y==v.y && z==v.z; }
    bool operator!=(const ivec &v) const { return x!=v.x || y!=v.y || z!=v.z; }
    ivec &mul(int n) { x *= n; y *= n; z *= n; return *this; }
    ivec &div(int n) { x /= n; y /= n; z /= n; return *this; }
    ivec &add(int n) { x += n; y += n; z += n; return *this; }
    ivec &sub(int n) { x -= n; y -= n; z -= n; return *this; }
    ivec &add(const ivec &v) { x += v.x; y += v.y; z += v.z; return *this; }
    ivec &sub(const ivec &v) { x -= v.x; y -= v.y; z -= v.z; return *this; }
    ivec &mask(int n) { x &= n; y &= n; z &= n; return *this; }
    ivec &cross(const ivec &a, const ivec &b) { x = a.y*b.z-a.z*b.y; y = a.z*b.x-a.x*b.z; z = a.x*b.y-a.y*b.x; return *this; }
    int dot(const ivec &o) const { return x*o.x + y*o.y + z*o.z; }
};

static inline bool htcmp(const ivec &x, const ivec &y)
{
    return x == y;
}

static inline uint hthash(const ivec &k)
{
    return k.x^k.y^k.z;
}

struct bvec
{
    union
    {
        struct { uchar x, y, z; };
        uchar v[3];
    };

    bvec() {}
    bvec(uchar x, uchar y, uchar z) : x(x), y(y), z(z) {}
    bvec(const vec &v) : x((uchar)((v.x+1)*255/2)), y((uchar)((v.y+1)*255/2)), z((uchar)((v.z+1)*255/2)) {}

    uchar &operator[](int i)       { return v[i]; }
    uchar  operator[](int i) const { return v[i]; }

    bool operator==(const bvec &v) const { return x==v.x && y==v.y && z==v.z; }
    bool operator!=(const bvec &v) const { return x!=v.x || y!=v.y || z!=v.z; }

    bool iszero() const { return x==0 && y==0 && z==0; }

    vec tovec() const { return vec(x*(2.0f/255.0f)-1.0f, y*(2.0f/255.0f)-1.0f, z*(2.0f/255.0f)-1.0f); }
};

struct quat
{
    float x, y, z, w;

    quat(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {}

    quat(const vec &axis, float angle)
    {
        w = cosf(angle/2);
        float s = sinf(angle/2);
        x = s*axis.x;
        y = s*axis.y;
        z = s*axis.z;
    }

    quat(float yaw, float pitch) // quat(vec(0, 1, 0), pitch).mul(quat((vec(0, 0, 1), yaw))
    {
        yaw *= RAD / 2;
        pitch *= RAD / 2;
        float yw = cosf(yaw), ys = sinf(yaw);
        float pw = cosf(pitch), ps = sinf(pitch);
        x = ps * ys;
        y = ps * yw;
        z = pw * ys;
        w = pw * yw;
    }

    quat &roll(float roll)  // quat(sinf(roll), 0, 0, cosf(roll)).mul(*this));
    {
        roll *= RAD / 2;
        float px = sinf(roll), pw = cosf(roll);
        quat o(*this);
        x = pw * o.x + px * o.w;
        y = pw * o.y - px * o.z;
        z = pw * o.z + px * o.y;
        w = pw * o.w - px * o.x;
        return *this;
    }
/*
    quat &mul(const quat &p, const quat &o)
    {
        x = p.w*o.x + p.x*o.w + p.y*o.z - p.z*o.y;
        y = p.w*o.y - p.x*o.z + p.y*o.w + p.z*o.x;
        z = p.w*o.z + p.x*o.y - p.y*o.x + p.z*o.w;
        w = p.w*o.w - p.x*o.x - p.y*o.y - p.z*o.z;
        return *this;
    }

    quat &mul(const quat &o) { return mul(quat(*this), o); }
*/
};

struct glmatrixf
{
    float v[16];

    float operator[](int i) const { return v[i]; }
    float &operator[](int i) { return v[i]; }

    #define ROTVEC(A, B) \
    { \
        float a = A, b = B; \
        A = a*c + b*s; \
        B = b*c - a*s; \
    }

    void rotate_around_x(float angle)
    {
        float c = cosf(angle), s = sinf(angle);
        ROTVEC(v[4], v[8]);
        ROTVEC(v[5], v[9]);
        ROTVEC(v[6], v[10]);
    }

    void rotate_around_y(float angle)
    {
        float c = cosf(angle), s = sinf(angle);
        ROTVEC(v[8], v[0]);
        ROTVEC(v[9], v[1]);
        ROTVEC(v[10], v[2]);
    }

    void rotate_around_z(float angle)
    {
        float c = cosf(angle), s = sinf(angle);
        ROTVEC(v[0], v[4]);
        ROTVEC(v[1], v[5]);
        ROTVEC(v[2], v[6]);
    }

    #undef ROTVEC

    #define MULMAT(row, col) \
       v[col + row] = x[row]*y[col] + x[row + 4]*y[col + 1] + x[row + 8]*y[col + 2] + x[row + 12]*y[col + 3];

    template<class XT, class YT>
    void mul(const XT x[16], const YT y[16])
    {
        MULMAT(0, 0); MULMAT(1, 0); MULMAT(2, 0); MULMAT(3, 0);
        MULMAT(0, 4); MULMAT(1, 4); MULMAT(2, 4); MULMAT(3, 4);
        MULMAT(0, 8); MULMAT(1, 8); MULMAT(2, 8); MULMAT(3, 8);
        MULMAT(0, 12); MULMAT(1, 12); MULMAT(2, 12); MULMAT(3, 12);
    }

    #undef MULMAT

    void mul(const glmatrixf &x, const glmatrixf &y)
    {
        mul(x.v, y.v);
    }

    void identity()
    {
        static const float m[16] =
        {
            1, 0, 0, 0,
            0, 1, 0, 0,
            0, 0, 1, 0,
            0, 0, 0, 1
        };
        memcpy(v, m, sizeof(v));
    }

    void translate(float x, float y, float z)
    {
        v[12] += x;
        v[13] += y;
        v[14] += z;
    }

    void translate(const vec &o)
    {
        translate(o.x, o.y, o.z);
    }

    void scale(float x, float y, float z)
    {
        v[0] *= x; v[1] *= x; v[2] *= x; v[3] *= x;
        v[4] *= y; v[5] *= y; v[6] *= y; v[7] *= y;
        v[8] *= z; v[9] *= z; v[10] *= z; v[11] *= z;
    }

    void projective()
    {
        loopi(2) loopj(4) v[i + j*4] = 0.5f*(v[i + j*4] + v[3 + j*4]);
    }

    void invertnormal(vec &dir) const
    {
        vec n(dir);
        dir.x = n.x*v[0] + n.y*v[1] + n.z*v[2];
        dir.y = n.x*v[4] + n.y*v[5] + n.z*v[6];
        dir.z = n.x*v[8] + n.y*v[9] + n.z*v[10];
    }

    void invertvertex(vec &pos) const
    {
        vec p(pos);
        p.x -= v[12];
        p.y -= v[13];
        p.z -= v[14];
        pos.x = p.x*v[0] + p.y*v[1] + p.z*v[2];
        pos.y = p.x*v[4] + p.y*v[5] + p.z*v[6];
        pos.z = p.x*v[8] + p.y*v[9] + p.z*v[10];
    }

    float transformx(const vec &p) const
    {
        return p.x*v[0] + p.y*v[4] + p.z*v[8] + v[12];
    }

    float transformy(const vec &p) const
    {
        return p.x*v[1] + p.y*v[5] + p.z*v[9] + v[13];
    }

    float transformz(const vec &p) const
    {
        return p.x*v[2] + p.y*v[6] + p.z*v[10] + v[14];
    }

    float transformw(const vec &p) const
    {
        return p.x*v[3] + p.y*v[7] + p.z*v[11] + v[15];
    }

    void transform(const vec &in, vec4 &out) const
    {
        out.x = transformx(in);
        out.y = transformy(in);
        out.z = transformz(in);
        out.w = transformw(in);
    }

    vec gettranslation() const
    {
        return vec(v[12], v[13], v[14]);
    }

    float determinant() const;
    void adjoint(const glmatrixf &m);
    bool invert(const glmatrixf &m, float mindet = 1.0e-10f);

    void fromquat(quat &q)
    {
        float xx = q.x * q.x, xy = q.x * q.y, xz = q.x * q.z, xw = q.x * q.w,
              yy = q.y * q.y, yz = q.y * q.z, yw = q.y * q.w,
              zz = q.z * q.z, zw = q.z * q.w;
        v[0]  = 1 - 2 * ( yy + zz );
        v[1]  =     2 * ( xy - zw );
        v[2]  =     2 * ( xz + yw );

        v[4]  =     2 * ( xy + zw );
        v[5]  = 1 - 2 * ( xx + zz );
        v[6]  =     2 * ( yz - xw );

        v[8]  =     2 * ( xz - yw );
        v[9]  =     2 * ( yz + xw );
        v[10] = 1 - 2 * ( xx + yy );

        v[3]  = v[7] = v[11] = v[12] = v[13] = v[14] = 0;
        v[15] = 1;
    }
};




