#include <iostream>
#include <vector>
#include <iomanip>
#include <fstream>
#include <sstream> // Include this header for stringstream
using namespace std;

struct SegmentTreeNode {
    int start, end, sum;
    SegmentTreeNode* left;
    SegmentTreeNode* right;

    SegmentTreeNode(int start, int end) : start(start), end(end), sum(0), left(nullptr), right(nullptr) {}
};

class SegmentTree {
private:
    SegmentTreeNode* root;

    SegmentTreeNode* buildTree(const vector<int>& nums, int start, int end) {
        if (start > end) return nullptr;
        SegmentTreeNode* node = new SegmentTreeNode(start, end);
        if (start == end) {
            node->sum = nums[start];
        }
        else {
            int mid = start + (end - start) / 2;
            node->left = buildTree(nums, start, mid);
            node->right = buildTree(nums, mid + 1, end);
            node->sum = node->left->sum + node->right->sum;
        }
        return node;
    }

    int SumHelper(SegmentTreeNode* node, int start, int end) {
        if (!node || start > node->end || end < node->start)
            return 0;
        if (start <= node->start && end >= node->end)
            return node->sum;
        return SumHelper(node->left, start, end) + SumHelper(node->right, start, end);

    }

    void updateHelper(SegmentTreeNode* node, int index, int newElement) {
        if (!node || index < node->start || index > node->end) return;
        if (node->start == node->end && node->start == index) {
            node->sum = newElement;
            return;
        }
        updateHelper(node->left, index, newElement);
        updateHelper(node->right, index, newElement);
        node->sum = node->left->sum + node->right->sum;
    }

    void printTreeHelper(SegmentTreeNode* node, ofstream& out, int& nullCount, string parentName = "") {
        if (node != nullptr) {
            stringstream ss;
            ss << "node" << nullCount++;
            string nodeName;
            ss >> nodeName;
            out << nodeName << " [label=\"" << node->sum << "\"];" << endl;
            if (!parentName.empty())
                out << parentName << " -> " << nodeName << ";" << endl;
            printTreeHelper(node->left, out, nullCount, nodeName);
            printTreeHelper(node->right, out, nullCount, nodeName);
        }
        else {
            stringstream ss;
            ss << "null" << nullCount++;
            string nullName;
            ss >> nullName;
            out << nullName << " [shape=point];" << endl;
            if (!parentName.empty())
                out << parentName << " -> " << nullName << ";" << endl;
        }
    }

public:

    SegmentTree(const vector<int>& nums) {
        int n = nums.size();
        root = buildTree(nums, 0, n - 1);
    }

    int SumElement(int start, int end) {
        return SumHelper(root, start, end);
    }

    void update(int index, int newElement) {
        updateHelper(root, index, newElement);
    }

    void printTreeToFile(const string& filename) {
        ofstream out(filename);
        out << "digraph SegmentTree {" << endl;
        int nullCount = 0;
        printTreeHelper(root, out, nullCount);
        out << "}" << endl;
        out.close();
        cout << "Graphviz DOT file has been generated: " << filename << endl;
    }

    void printGraphToFile() {
        printTreeToFile("graph.dot");
    }
};

int main() {

    vector<int> nums = { 1, 3, 5, 7, 9, 11,24,-5,35,-6564,-6,4,6,245,36,546,546,546,54,6546,546,0 };
    SegmentTree segTree(nums);

    cout << "Sum element: " << segTree.SumElement(1, 3) << endl;

    segTree.update(2, 10); // Changing the value at index 2 to 10

    segTree.printGraphToFile();

    cout << "\nSegment Tree after update:\n";

    cout << "\n\n";

    cout << "Sum elements update: " << segTree.SumElement(1, 3) << endl;

    system("dot -Tpng -O graph.dot");
    system("graph.dot.png");

    return 0;
}
