/*
 * scene_graph.cpp
 *
 * Author: Charles Magahern <charles@magahern.com>
 * Date Created: 04/02/2012
 */
 
#include "scene_graph.h"
#include <queue>
#include <ctime>
#include <cstdlib>

SceneGraphNode::SceneGraphNode() :
    transform(Transform::identityTransform()),
    parent(NULL),
    _marked(false),
    _uuid(rand()) {}

SceneGraphNode::~SceneGraphNode()
{
    std::vector<SceneGraphNode *>::iterator itr;
    for (itr = children.begin(); itr != children.end(); ++itr) {
        delete *itr;
    }
}

void SceneGraphNode::addChild(SceneGraphNode *node)
{
    node->parent = this;
    children.push_back(node);
}

void SceneGraphNode::visit()
{
    glMultMatrixf(transform.matrix);
}

bool SceneGraphNode::isEqual(SceneGraphNode *other)
{
    return _uuid == other->_uuid;
}

SceneGraphNode* SceneGraphNode::getLeftSibling(SceneGraphNode *root, SceneGraphNode *target)
{
    if (root == NULL || target == NULL) return NULL;

    SceneGraphNode *previous = NULL;

    std::queue<SceneGraphNode *> queue;
    queue.push(root);
    root->_marked = true;

    while (!queue.empty()) {
        SceneGraphNode *t = queue.front(); queue.pop();
        if (t->isEqual(target)) {
            break;
        } else {
            previous = t;
        }

        std::vector<SceneGraphNode *>::iterator itr;
        for (itr = t->children.begin(); itr != t->children.end(); ++itr) {
            if (!(*itr)->_marked) {
                (*itr)->_marked = true;
                queue.push(*itr);
            }
        }
    }

    _clearMarked(root);
    return previous;
}

SceneGraphNode* SceneGraphNode::getRightSibling(SceneGraphNode *root, SceneGraphNode *target)
{
    if (root == NULL || target == NULL) return NULL;
    
    SceneGraphNode *result = NULL;
    bool foundSelf = false;
    
    std::queue<SceneGraphNode *> queue;
    queue.push(root);
    root->_marked = true;
    
    while (!queue.empty()) {
        SceneGraphNode *t = queue.front(); queue.pop();
        if (foundSelf) {
            result = t;
            break;
        }
        
        if (t->isEqual(target)) {
            foundSelf = true;
        }
        
        std::vector<SceneGraphNode *>::iterator itr;
        for (itr = t->children.begin(); itr != t->children.end(); ++itr) {
            if (!(*itr)->_marked) {
                (*itr)->_marked = true;
                queue.push(*itr);
            }
        }
    }
    
    _clearMarked(root);
    return result;
}

Transform SceneGraphNode::aggregateTransform()
{
    Transform t = Transform::identityTransform();
    return _aggregateTransform(this, &t);
}

Transform SceneGraphNode::_aggregateTransform(SceneGraphNode *node, Transform *t)
{
    if (node == NULL) return *t;
    
    *t *= transform;
    return _aggregateTransform(node->parent, t);
}

void SceneGraphNode::_clearMarked(SceneGraphNode *root)
{
    root->_marked = false;
    std::vector<SceneGraphNode *>::iterator itr;
    for (itr = root->children.begin(); itr != root->children.end(); ++itr) {
        _clearMarked(*itr);
    }
}
