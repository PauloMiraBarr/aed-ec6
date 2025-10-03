#include <iostream>
#include <vector>
#include <queue>
#include <optional>
#include <cmath>
#include <string>
using namespace std;

struct TreeNode {
    int val;
    TreeNode *left, *right;
    TreeNode(int v) : val(v), left(nullptr), right(nullptr) {}
};

// ====== Código bajo prueba ======
class Solution {
public:

    int altura(TreeNode* root) {
        if (!root) {
            return 0;
        }

        int hl = altura(root->left);
        if (hl == -1){
            return -1;  
        } 

        int hr = altura(root->right);
        if (hr == -1) {
            return -1; 
        }

        if (abs(hl - hr) > 1) {
            return -1; 
        }

        return 1 + max(hl, hr);  
    }

    
    bool problema1(TreeNode* root) {
        return altura(root) != -1;
    }

};

// ====== Helpers para pruebas ======
TreeNode* buildTree(const vector<optional<int>>& lvl) {
    if (lvl.empty() || !lvl[0].has_value()) return nullptr;
    vector<TreeNode*> nodes(lvl.size(), nullptr);
    for (size_t i = 0; i < lvl.size(); ++i) {
        if (lvl[i].has_value()) nodes[i] = new TreeNode(*lvl[i]);
    }
    for (size_t i = 0; i < lvl.size(); ++i) {
        if (!nodes[i]) continue;
        size_t L = 2*i + 1, R = 2*i + 2;
        if (L < lvl.size()) nodes[i]->left  = nodes[L];
        if (R < lvl.size()) nodes[i]->right = nodes[R];
    }
    return nodes[0];
}

void freeTree(TreeNode* root) {
    if (!root) return;
    queue<TreeNode*> q; q.push(root);
    while (!q.empty()) {
        TreeNode* n = q.front(); q.pop();
        if (n->left) q.push(n->left);
        if (n->right) q.push(n->right);
        delete n;
    }
}

struct TestResult { int passed=0, total=0; };

void expectEq(bool got, bool expected, const string& name, TestResult& tr) {
    ++tr.total;
    if (got == expected) {
        ++tr.passed;
        cout << "[OK]   " << name << "\n";
    } else {
        cout << "[FAIL] " << name << "  expected=" << (expected?"true":"false")
             << " got=" << (got?"true":"false") << "\n";
    }
}

// ====== 10 pruebas ======
int main() {
    Solution sol;
    TestResult tr;

    // 1) Árbol vacío -> balanceado
    expectEq(sol.problema1(nullptr), true, "Test 1: empty tree", tr);

    // 2) Un solo nodo
    {
        TreeNode* root = buildTree({1});
        expectEq(sol.problema1(root), true, "Test 2: single node", tr);
        freeTree(root);
    }

    // 3) Perfecto de altura 2
    {
        TreeNode* root = buildTree({1,2,3,4,5,6,7});
        expectEq(sol.problema1(root), true, "Test 3: perfect height-2", tr);
        freeTree(root);
    }

    // 4) Completamente inclinado derecha
    {
        TreeNode* root = new TreeNode(1);
        root->right = new TreeNode(2);
        root->right->right = new TreeNode(3);
        root->right->right->right = new TreeNode(4);
        expectEq(sol.problema1(root), false, "Test 4: right-skewed depth 4", tr);
        freeTree(root);
    }

    // 5) Diferencia 1 en raíz
    {
        TreeNode* root = buildTree({1,2,3,4});
        expectEq(sol.problema1(root), true, "Test 5: root diff=1", tr);
        freeTree(root);
    }

    // 6) Diferencia 2 en raíz
    {
        TreeNode* root = buildTree({1,2,3,4, nullopt, nullopt, nullopt, 5});
        expectEq(sol.problema1(root), false, "Test 6: root diff=2", tr);
        freeTree(root);
    }

    // 7) Desbalance profundo en subárbol izquierdo
    {
        TreeNode* root = new TreeNode(10);
        root->left = new TreeNode(5);
        root->right = new TreeNode(15);
        root->left->left = new TreeNode(3);
        root->left->left->left = new TreeNode(1);
        expectEq(sol.problema1(root), false, "Test 7: deep left unbalanced", tr);
        freeTree(root);
    }

    // 8) Hueco interno pero balanceado
    {
        TreeNode* root = buildTree({1,2,3,4, nullopt, nullopt, 6});
        expectEq(sol.problema1(root), true, "Test 8: internal gap balanced", tr);
        freeTree(root);
    }

    // 9) Sub-subárbol con diff=2
    {
        TreeNode* root = buildTree({1,2,3,4,5,nullopt,nullopt,6});
        expectEq(sol.problema1(root), false, "Test 9: sub-subtree diff=2", tr);
        freeTree(root);
    }

    // 10) Asimétrico pero balanceado
    {
        TreeNode* root = new TreeNode(1);
        root->left = new TreeNode(2);
        root->right = new TreeNode(3);
        root->left->right = new TreeNode(4);
        root->right->right = new TreeNode(5);
        expectEq(sol.problema1(root), true, "Test 10: asymmetric yet balanced", tr);
        freeTree(root);
    }

    cout << "\nResumen: " << tr.passed << "/" << tr.total << " tests OK\n";
    return (tr.passed == tr.total) ? 0 : 1;
}
