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
    TreeNode() : val(0), left(nullptr), right(nullptr) {}
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
    TreeNode(int x, TreeNode* l, TreeNode* r) : val(x), left(l), right(r) {}
};

class Solution {
public:
    TreeNode* problema2(TreeNode* root) {
        if(root ==nullptr){
            return nullptr;
        }
        vector<int> inorder;
        
        Recorrer_inorder(root, inorder);

        int t = inorder.size();

        return convert(inorder, 0, t - 1);
    }
    TreeNode* Recorrer_inorder(TreeNode* root, vector<int>& inorder) {
        if (root == nullptr) return nullptr;
        
        Recorrer_inorder(root->left, inorder);
        
        inorder.push_back(root->val);
        
        Recorrer_inorder(root->right, inorder);
        
        return nullptr;
    }

    TreeNode* convert(vector<int>& inorder, int i, int j) {
        if (i > j) return nullptr;
        
        int mitad = i + (j - i) / 2;
        
        TreeNode* ls = convert(inorder, i, mitad - 1);
        TreeNode* rs = convert(inorder, mitad + 1, j);
        
        TreeNode* root = new TreeNode(inorder[mitad], ls, rs);
        
        return root;
    }
};

// ===== Helper para verificar balance =====
pair<bool,int> dfsCheck(TreeNode* node){
    if(!node) return {true,0};
    auto [lb,lh] = dfsCheck(node->left);
    auto [rb,rh] = dfsCheck(node->right);
    bool ok = lb && rb && abs(lh-rh)<=1;
    return {ok,1+max(lh,rh)};
}
bool isBalanced(TreeNode* root){ return dfsCheck(root).first; }

// ===== Helpers para construir y liberar =====
TreeNode* buildBSTfromSorted(const vector<int>& arr, int l, int r){
    if(l>r) return nullptr;
    int m = (l+r)/2;
    TreeNode* node = new TreeNode(arr[m]);
    node->left = buildBSTfromSorted(arr,l,m-1);
    node->right = buildBSTfromSorted(arr,m+1,r);
    return node;
}
TreeNode* buildRightSkewed(const vector<int>& vals){
    TreeNode* root = nullptr; TreeNode* cur = nullptr;
    for(int v: vals){
        if(!root){ root=new TreeNode(v); cur=root; }
        else { cur->right=new TreeNode(v); cur=cur->right; }
    }
    return root;
}
void freeTree(TreeNode* root){
    if(!root) return;
    queue<TreeNode*> q; q.push(root);
    while(!q.empty()){
        TreeNode* n=q.front(); q.pop();
        if(n->left) q.push(n->left);
        if(n->right) q.push(n->right);
        delete n;
    }
}

// ====== Test harness ======
struct TestResult { int passed=0,total=0; };
void expectEq(bool got, bool expected, const string& name, TestResult& tr){
    ++tr.total;
    if(got==expected){ ++tr.passed; cout<<"[OK]   "<<name<<"\n"; }
    else { cout<<"[FAIL] "<<name<<" expected="<<(expected?"true":"false")<<" got="<<(got?"true":"false")<<"\n"; }
}

int main(){
    Solution sol;
    TestResult tr;

    // 1) Árbol vacío
    expectEq(isBalanced(sol.problema2(nullptr)), true, "Test 1: empty tree", tr);

    // 2) Un solo nodo
    {
        TreeNode* root=new TreeNode(1);
        TreeNode* res=sol.problema2(root);
        expectEq(isBalanced(res), true, "Test 2: single node", tr);
        freeTree(res); freeTree(root);
    }

    // 3) Árbol ya balanceado (perfecto)
    {
        TreeNode* root=buildBSTfromSorted({1,2,3,4,5,6,7},0,6);
        TreeNode* res=sol.problema2(root);
        expectEq(isBalanced(res), true, "Test 3: perfect BST remains balanced", tr);
        freeTree(res); freeTree(root);
    }

    // 4) Árbol lineal hacia la derecha (muy desbalanceado)
    {
        TreeNode* root=buildRightSkewed({1,2,3,4,5,6,7});
        TreeNode* res=sol.problema2(root);
        expectEq(isBalanced(res), true, "Test 4: right-skewed balanced", tr);
        freeTree(res); freeTree(root);
    }

    // 5) Árbol lineal hacia la derecha con 3 nodos
    {
        TreeNode* root=buildRightSkewed({1,2,3});
        TreeNode* res=sol.problema2(root);
        expectEq(isBalanced(res), true, "Test 5: right-skewed 3 nodes balanced", tr);
        freeTree(res); freeTree(root);
    }

    // 6) Árbol lineal con 2 nodos
    {
        TreeNode* root=buildRightSkewed({10,20});
        TreeNode* res=sol.problema2(root);
        expectEq(isBalanced(res), true, "Test 6: two nodes balanced", tr);
        freeTree(res); freeTree(root);
    }

    // 7) Árbol ya balanceado con 2 niveles
    {
        TreeNode* root=buildBSTfromSorted({1,2,3},0,2);
        TreeNode* res=sol.problema2(root);
        expectEq(isBalanced(res), true, "Test 7: small balanced", tr);
        freeTree(res); freeTree(root);
    }

    // 8) Árbol desbalanceado con 4 nodos en línea
    {
        TreeNode* root=buildRightSkewed({1,2,3,4});
        TreeNode* res=sol.problema2(root);
        expectEq(isBalanced(res), true, "Test 8: line of 4 balanced", tr);
        freeTree(res); freeTree(root);
    }

    // 9) Árbol con valores negativos
    {
        TreeNode* root=buildRightSkewed({-10,-3,0,5,9});
        TreeNode* res=sol.problema2(root);
        expectEq(isBalanced(res), true, "Test 9: negative values BST balanced", tr);
        freeTree(res); freeTree(root);
    }

    // 10) Árbol grande desbalanceado (10 nodos lineales)
    {
        vector<int> vals; for(int i=1;i<=10;i++) vals.push_back(i);
        TreeNode* root=buildRightSkewed(vals);
        TreeNode* res=sol.problema2(root);
        expectEq(isBalanced(res), true, "Test 10: big skewed balanced", tr);
        freeTree(res); freeTree(root);
    }

    cout<<"\nResumen: "<<tr.passed<<"/"<<tr.total<<" tests OK\n";
    return (tr.passed==tr.total)?0:1;
}
