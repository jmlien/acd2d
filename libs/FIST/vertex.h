#ifdef EXT_APPL_SITE

#define SetExtApplVtx(V, X) \
{\
    assert(InVertexList(V)); \
    vertices[V].ext_appl = X; \
}


#define GetExtApplVtx(V) \
(\
    assert(InVertexList(V)), \
    vertices[V].ext_appl)

#endif
