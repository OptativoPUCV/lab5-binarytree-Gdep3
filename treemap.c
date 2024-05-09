#include "treemap.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct TreeNode TreeNode;

struct TreeNode {
  Pair *pair;
  TreeNode *left;
  TreeNode *right;
  TreeNode *parent;
};

struct TreeMap {
  TreeNode *root;
  TreeNode *current;
  int (*lower_than)(void *key1, void *key2);
};

int is_equal(TreeMap *tree, void *key1, void *key2) {
  if (tree->lower_than(key1, key2) == 0 && tree->lower_than(key2, key1) == 0)
    return 1;
  else
    return 0;
}

TreeNode *createTreeNode(void *key, void *value) {
  TreeNode *new = (TreeNode *)malloc(sizeof(TreeNode));
  if (new == NULL)
    return NULL;
  new->pair = (Pair *)malloc(sizeof(Pair));
  new->pair->key = key;
  new->pair->value = value;
  new->parent = new->left = new->right = NULL;
  return new;
}

TreeMap *createTreeMap(int (*lower_than)(void *key1, void *key2)) {
  TreeMap *new = (TreeMap *)malloc(sizeof(TreeMap));
  new->root = NULL;
  new->current = NULL;
  new->lower_than = lower_than;
  return new;
  // new->lower_than = lower_than;
}

void insertTreeMap(TreeMap *tree, void *key, void *value) {
  if (searchTreeMap(tree, key) == NULL) {
    TreeNode *aux = tree->root;
    TreeNode *parent = NULL;
    while (aux != NULL) {
      parent = aux;
      if (tree->lower_than(key, aux->pair->key)) {
        aux = aux->left;
      } else {
        aux = aux->right;
      }
    }
    TreeNode *newNode = createTreeNode(key, value);
    newNode->parent = parent;
    if (parent == NULL) {
      tree->root = newNode;
      tree->current = newNode;
    } else if (tree->lower_than(key, parent->pair->key)) {
      parent->left = newNode;
      tree->current = newNode;
    } else {
      parent->right = newNode;
      tree->current = newNode;
    }
  }
}

TreeNode *minimum(TreeNode *x) {
  while (x->left != NULL) {
    x = x->left;
  }
  return x;
}

void removeNode(TreeMap *tree, TreeNode *node) {
  if (node == NULL)
    return;

  if (node->left == NULL && node->right == NULL) {
    if (node->parent != NULL) {
      if (node->parent->left == node) {
        node->parent->left = NULL;
      } else {
        node->parent->right = NULL;
      }
    } else {
      tree->root = NULL;
    }
    free(node);
  } else if (node->left == NULL || node->right == NULL) {
    TreeNode *child = (node->left != NULL) ? node->left : node->right;
    if (node->parent != NULL) {
      if (node->parent->left == node) {
        node->parent->left = child;
      } else {
        node->parent->right = child;
      }
      child->parent = node->parent;
    } else {
      tree->root = child;
      child->parent = NULL;
    }
    free(node);
  } else {
    TreeNode *aux = minimum(node->right);
    node->pair = aux->pair;
    removeNode(tree, aux);
  }
}

void eraseTreeMap(TreeMap *tree, void *key) {
  if (tree == NULL || tree->root == NULL)
    return;

  if (searchTreeMap(tree, key) == NULL)
    return;
  TreeNode *node = tree->current;
  removeNode(tree, node);
}

Pair *searchTreeMap(TreeMap *tree, void *key) {
  TreeNode *aux = tree->root;
  while (aux != NULL) {
    if (is_equal(tree, key, aux->pair->key)) {
      tree->current = aux;
      return aux->pair;
    } else if (tree->lower_than(key, aux->pair->key) == 0) {
      aux = aux->right;
    } else if (tree->lower_than(key, aux->pair->key) == 1) {
      aux = aux->left;
    }
  }
  return NULL;
}

Pair *upperBound(TreeMap *tree, void *key) { return NULL; }

Pair *firstTreeMap(TreeMap *tree) {
  TreeNode *aux = tree->root;
  while (aux->left != NULL) {
    aux = aux->left;
  }
  return aux->pair;
}

Pair *nextTreeMap(TreeMap *tree) {
  TreeNode *aux = tree->current;
  if (aux->right != NULL) {
    aux = minimum(aux->right);
    return aux->pair;
  } else {
    while (aux != NULL && tree->lower_than(aux, tree->current) == 1) {
      aux = aux->parent;
    }
    if (aux != NULL)
      return aux->pair;
    else
      return NULL;
  }
}
