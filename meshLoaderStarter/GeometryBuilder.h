#ifndef GEOMETRYBUILDER_H
#define GEOMETRYBUILDER_H

class MeshMgr;
class GameObject_t;
class Mesh_t;

/*
* build the geometry for some common shapes and store in the MeshMgr library
* only one of each allowed, identified by text name
*/

Mesh_t& BuildQuad(MeshMgr& mgr);
Mesh_t& BuildPyramid(MeshMgr& mgr);
Mesh_t& BuildCube(MeshMgr& mgr);
Mesh_t& BuildSphere(MeshMgr& mgr, int LatLines, int LongLines);

#endif
