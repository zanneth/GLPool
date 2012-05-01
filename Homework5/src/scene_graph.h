/*
 * scene_graph.h
 *
 * Author: Charles Magahern <charles@magahern.com>
 * Date Created: 04/02/2012
 */
 
#pragma once

#include "picture.h"
#include "geometry.h"
#include "transform.h"
#include <iostream>
#include <vector>

typedef unsigned NodeUUID;

struct SceneGraphNode {
    Transform transform;
    Picture picture;
    
    SceneGraphNode *parent;
    std::vector<SceneGraphNode *> children;

protected:
    bool _marked; // For BFS
    NodeUUID _uuid;
    
public:
    SceneGraphNode();
    ~SceneGraphNode();
    
    void addChild(SceneGraphNode *node);
    virtual void visit();
    
    bool isEqual(SceneGraphNode *other);
    
    static SceneGraphNode* getLeftSibling(SceneGraphNode *root, SceneGraphNode *target);
    static SceneGraphNode* getRightSibling(SceneGraphNode *root, SceneGraphNode *target);
    
    Transform aggregateTransform();
    
private:
    Transform _aggregateTransform(SceneGraphNode *node, Transform *t);
    static void _clearMarked(SceneGraphNode *root);
};
