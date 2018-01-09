#define SetExtApplPnt(P, X) \
{\
    assert(InPointsList(P)); \
    points[P].ext_appl = X; \
}


#define GetExtApplPnt(P) \
(\
    assert(InPointsList(P)), \
    points[P].ext_appl)
