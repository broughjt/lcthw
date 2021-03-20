#include <stdlib.h>
#include <lcthw/debug.h>
#include <lcthw/bstree.h>
#include <lcthw/bstrlib.h>

static int default_compare(void *a, void *b)
{
  return bstrcmp((bstring) a, (bstring) b);
}

BSTree *BSTree_create(BSTree_compare compare)
{
  BSTree *self = malloc(sizeof(BSTree));
  check_mem(self);

  self->compare = compare == NULL ? default_compare : compare;
  self->count = 0;
  self->root = NULL;

  return self;

error:
  return NULL;
}

static int destroy_callback(BSTreeNode *node)
{
  free(node);
  return 0;
}

void BSTree_destroy(BSTree *self)
{
  if (self) {
    if (self->root)
      BSTree_traverse_post_order(self, destroy_callback);
    free(self);
  }
}

static inline BSTreeNode *BSTreeNode_create(BSTreeNode *parent, void *key, void *value)
{
  BSTreeNode *self = malloc(sizeof(BSTreeNode));
  check_mem(self);

  self->key = key;
  self->value = value;
  self->parent = parent;
  self->left = NULL;
  self->right = NULL;

  return self;

error:
  return NULL;
}

BSTreeNode *BSTree_set(BSTree *self, void *key, void *value)
{
  BSTreeNode *parent = NULL;
  BSTreeNode *node = self->root;
  int comparison;

  while (node) {
    comparison = self->compare(node->key, key);
    parent = node;
    node = comparison <= 0 ? node->left : node->right;
  }

  BSTreeNode *child = BSTreeNode_create(parent, key, value);

  if (!parent)
    self->root = child;
  else if (comparison <= 0)
    parent->left = child;
  else
    parent->right = child;

  self->count++;

  return child;
}

static inline BSTreeNode *BSTree_get_node(BSTree *self, BSTreeNode *node, void *key)
{
  while (node) {
    int comparison = self->compare(node->key, key);
    if (comparison == 0)
      return node;
    node = comparison < 0 ? node->left : node->right;
  }

  return NULL;
}

void *BSTree_get(BSTree *self, void *key)
{
  BSTreeNode *node = BSTree_get_node(self, self->root, key);

  return node == NULL ? NULL : node->value;
}

static inline int BSTree_traverse_nodes_in_order(BSTreeNode *node, BSTree_traverse_callback callback)
{
  int result = 0;

  if (node->left) {
    result = BSTree_traverse_nodes_in_order(node->left, callback);
    if (result != 0)
      return result;
  }

  result = callback(node);
  if (result != 0)
    return result;

  if (node->right) {
    result = BSTree_traverse_nodes_in_order(node->right, callback);
    if (result != 0)
      return result;
  }

  return result;
}

static inline int BSTree_traverse_nodes_pre_order(BSTreeNode *node, BSTree_traverse_callback callback)
{
  int result = callback(node);
  if (result != 0)
    return result;

  if (node->left) {
    result = BSTree_traverse_nodes_pre_order(node->left, callback);
    if (result != 0)
      return result;
  }

  if (node->right) {
    result = BSTree_traverse_nodes_pre_order(node->right, callback);
    if (result != 0)
      return result;
  }

  return 0;
}

static inline int BSTree_traverse_nodes_post_order(BSTreeNode *node, BSTree_traverse_callback callback)
{
  int result = 0;

  if (node->left) {
    result = BSTree_traverse_nodes_pre_order(node->left, callback);
    if (result != 0)
      return result;
  }

  if (node->right) {
    result = BSTree_traverse_nodes_pre_order(node->right, callback);
    if (result != 0)
      return result;
  }

  return callback(node);
}

int BSTree_traverse_in_order(BSTree *self, BSTree_traverse_callback callback)
{
  if (self->root)
    return BSTree_traverse_nodes_in_order(self->root, callback);
  return 0;
}

int BSTree_traverse_pre_order(BSTree *self, BSTree_traverse_callback callback)
{
  if (self->root)
    return BSTree_traverse_nodes_pre_order(self->root, callback);
  return 0;
}

int BSTree_traverse_post_order(BSTree *self, BSTree_traverse_callback callback)
{
  if (self->root)
    return BSTree_traverse_nodes_post_order(self->root, callback);
  return 0;
}

int BSTree_traverse(BSTree *self, BSTree_traverse_callback callback)
{
  if (self->root)
    return BSTree_traverse_nodes_in_order(self->root, callback);
  return 0;
}

static inline BSTreeNode *BSTree_find_minimum(BSTreeNode *node)
{
  while (node->left)
    node = node->left;

  return node;
}

static inline void BSTree_replace_in_parent(BSTree *self, BSTreeNode *node, BSTreeNode *new_value)
{
  if (node->parent) {
    if (node == node->parent->left)
      node->parent->left = new_value;
    else
      node->parent->right = new_value;
  } else {
    self->root = new_value;
  }

  if (new_value)
    new_value->parent = node->parent;
}

static inline void BSTree_swap(BSTreeNode *a, BSTreeNode *b)
{
  void *t = b->key;
  b->key = a->key;
  a->key = t;

  t = b->value;
  b->value = a->value;
  a->value = t;
}

static inline BSTreeNode *BSTree_delete_node(BSTree *self, BSTreeNode *node, void *key)
{
  int comparison = self->compare(node->key, key);

  if (comparison < 0) {
    return node->left ? BSTree_delete_node(self, node->left, key) : NULL;
  } else if (comparison > 0) {
    return node->right ? BSTree_delete_node(self, node->right, key) : NULL;
  } else {
    if (node->left && node->right) {
      BSTreeNode *successor = BSTree_find_minimum(node->right);
      BSTree_swap(successor, node);
      BSTree_replace_in_parent(self, successor, successor->right);

      return successor;
    } else if (node->left) {
      BSTree_replace_in_parent(self, node, node->left);
    } else if (node->right) {
      BSTree_replace_in_parent(self, node, node->right);
    } else {
      BSTree_replace_in_parent(self, node, NULL);
    }

    return node;
  }
}

void *BSTree_delete(BSTree *self, void *key)
{
  void *value = NULL;

  if (self->root) {
    BSTreeNode *node = BSTree_delete_node(self, self->root, key);

    if (node) {
      value = node->value;
      free(node);
    }
  }

  return value;
}
