#include <bits/stdc++.h>
using namespace std;

struct TreeNode {
    int val;
    TreeNode *left, *right;
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
};




// ====== Código bajo prueba ======
class Solution {
public:
    
    TreeNode* problema3(TreeNode* root, TreeNode* p, TreeNode* q) {
        if(!root) return nullptr;
        // Caso 1: p < root y root < q (root es respuesta)
        if(p->val < root->val and root->val < q->val)
            return root;
        // Caso 1.b: q < root y root < p (mismo caso que el 1)
        if(q->val < root->val and root->val < p->val)
            return root;
        // Caso 2: root < p y root < q (moverse a la derecha)
        if(root->val < p->val and root->val < q->val)
            return problema3(root->right, p, q);
        // Caso 3: root > p y root > q (moverse a la izquierda)
        if(root->val > p->val and root->val > q->val)
            return problema3(root->left, p, q);
        // Caso 4: root = p o root = q (uno es el ancestro de otro)
        if(root->val == p->val or root->val == q->val)
            return root;
    }
};





// ====== Helpers de pruebas ======
TreeNode* insertBST(TreeNode* root, int v){
    if(!root) return new TreeNode(v);
    if(v < root->val) root->left = insertBST(root->left, v);
    else              root->right = insertBST(root->right, v);
    return root;
}
TreeNode* buildBST(const vector<int>& vals){
    TreeNode* root=nullptr;
    for(int v: vals) root=insertBST(root, v);
    return root;
}
TreeNode* findNode(TreeNode* root, int v){
    while(root){
        if(v==root->val) return root;
        root = (v < root->val) ? root->left : root->right;
    }
    return nullptr; // No usar en tests donde no exista
}
void freeTree(TreeNode* root){
    if(!root) return;
    stack<TreeNode*> st; st.push(root);
    while(!st.empty()){
        TreeNode* n=st.top(); st.pop();
        if(n->left) st.push(n->left);
        if(n->right) st.push(n->right);
        delete n;
    }
}

// Pequeño framework
struct TestResult { int passed=0, total=0; };
void expectEqPtr(TreeNode* got, TreeNode* expected, const string& name, TestResult& tr){
    ++tr.total;
    if(got==expected){ ++tr.passed; cout<<"[OK]   "<<name<<"\n"; }
    else {
        cout<<"[FAIL] "<<name<<" expected="<<(expected?to_string(expected->val):"null")
            <<" got="<<(got?to_string(got->val):"null")<<"\n";
    }
}

// ====== 10 pruebas ======
int main(){
    Solution sol;
    TestResult tr;

    // 1) Caso básico: LCA en la raíz
    // BST: [2,1,3] -> LCA(1,3)=2
    {
        TreeNode* root = buildBST({2,1,3});
        auto p = findNode(root,1);
        auto q = findNode(root,3);
        expectEqPtr(sol.problema3(root,p,q), root, "Test 1: LCA(1,3)=2", tr);
        freeTree(root);
    }

    // 2) Mismo nodo: p==q
    // LCA(2,2)=2
    {
        TreeNode* root = buildBST({2,1,3});
        auto p = findNode(root,2);
        expectEqPtr(sol.problema3(root,p,p), p, "Test 2: p==q==2", tr);
        freeTree(root);
    }

    // 3) Ambos en el subárbol izquierdo
    // Ejemplo clásico: [6,2,8,0,4,7,9,3,5] -> LCA(2,4)=2
    {
        TreeNode* root = buildBST({6,2,8,0,4,7,9,3,5});
        auto p = findNode(root,2);
        auto q = findNode(root,4);
        expectEqPtr(sol.problema3(root,p,q), p, "Test 3: ambos izquierda, LCA=2", tr);
        freeTree(root);
    }

    // 4) Ambos en el subárbol derecho
    // LCA(7,9)=8
    {
        TreeNode* root = buildBST({6,2,8,0,4,7,9,3,5});
        auto p = findNode(root,7);
        auto q = findNode(root,9);
        auto expected = findNode(root,8);
        expectEqPtr(sol.problema3(root,p,q), expected, "Test 4: ambos derecha, LCA=8", tr);
        freeTree(root);
    }

    // 5) Uno es ancestro del otro (ancestro a mitad)
    // LCA(3,5)=4
    {
        TreeNode* root = buildBST({6,2,8,0,4,7,9,3,5});
        auto p = findNode(root,3);
        auto q = findNode(root,5);
        auto expected = findNode(root,4);
        expectEqPtr(sol.problema3(root,p,q), expected, "Test 5: ancestro intermedio, LCA=4", tr);
        freeTree(root);
    }

    // 6) p es la raíz
    // LCA(6,9)=6
    {
        TreeNode* root = buildBST({6,2,8,0,4,7,9});
        auto p = findNode(root,6);
        auto q = findNode(root,9);
        expectEqPtr(sol.problema3(root,p,q), p, "Test 6: p es raíz, LCA=6", tr);
        freeTree(root);
    }

    // 7) Árbol lineal a la derecha
    // 1-2-3-4-5 -> LCA(2,5)=2
    {
        TreeNode* root = buildBST({1,2,3,4,5});
        auto p = findNode(root,2);
        auto q = findNode(root,5);
        expectEqPtr(sol.problema3(root,p,q), p, "Test 7: skewed derecha, LCA=2", tr);
        freeTree(root);
    }

    // 8) Árbol lineal a la izquierda
    // 5-4-3-2-1 -> LCA(1,4)=4
    {
        TreeNode* root = buildBST({5,4,3,2,1});
        auto p = findNode(root,1);
        auto q = findNode(root,4);
        auto expected = findNode(root,4);
        expectEqPtr(sol.problema3(root,p,q), expected, "Test 8: skewed izquierda, LCA=4", tr);
        freeTree(root);
    }

    // 9) Valores negativos y positivos
    // Inserción: -10 -> -3 -> 0 -> 5 -> 9  (cadena hacia la derecha)
    // LCA(-3,9) = -3 (p es ancestro de q)
    {
        TreeNode* root = buildBST({-10,-3,0,5,9});
        auto p = findNode(root,-3);
        auto q = findNode(root,9);
        auto expected = findNode(root,-3);  // <-- corregido
        expectEqPtr(sol.problema3(root,p,q), expected, "Test 9: mezcla neg/pos, LCA=-3", tr);
        freeTree(root);
    }

    // 10) Árbol más grande con ramas simétricas
    // [20,10,30,5,15,25,35,13,17] -> LCA(13,17)=15
    {
        TreeNode* root = buildBST({20,10,30,5,15,25,35,13,17});
        auto p = findNode(root,13);
        auto q = findNode(root,17);
        auto expected = findNode(root,15);
        expectEqPtr(sol.problema3(root,p,q), expected, "Test 10: LCA(13,17)=15", tr);
        freeTree(root);
    }

    cout << "\nResumen: " << tr.passed << "/" << tr.total << " tests OK\n";
    return (tr.passed==tr.total)?0:1;
}
