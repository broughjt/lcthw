#ifndef _lcthw_BSTree_h
#define _lcthw_BSTree_h

typedef int (*BSTree_compare) (void *a, void *b);

typedef struct BSTreeNode {
  void *key;
  void *value;

  struct BSTreeNode *left;
  struct BSTreeNode *right;
  struct BSTreeNode *parent;
} BSTreeNode;

typedef struct BSTree {
  int count;
  BSTree_compare compare;
  BSTreeNode *root;
} BSTree;

typedef int (*BSTree_traverse_callback) (BSTreeNode *node);

BSTree *BSTree_create(BSTree_compare compare);

void BSTree_destroy(BSTree *self);

BSTreeNode *BSTree_set(BSTree *self, void *key, void *value);

void *BSTree_get(BSTree *self, void *key);

int BSTree_traverse_in_order(BSTree *self, BSTree_traverse_callback callback);

int BSTree_traverse_pre_order(BSTree *self, BSTree_traverse_callback callback);

int BSTree_traverse_post_order(BSTree *self, BSTree_traverse_callback callback);

int BSTree_traverse(BSTree *self, BSTree_traverse_callback callback);

void *BSTree_delete(BSTree *self, void *key);

#endif