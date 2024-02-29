#include <iostream>
#include <fstream> 
#include <sstream> 
#include <vector>
#include <cmath>

using namespace std;

struct Node {
    int coefficient;
    int exponent;    
    Node* next;

    Node(int coef, int exp) : coefficient(coef), exponent(exp), next(nullptr) {}
};

class Polynomial {
private:
    Node* head;

public:

    Polynomial() : head(nullptr) {}

    void addTerm(int coefficient, int exponent) {
        Node* newNode = new Node(coefficient, exponent);
        if (head == nullptr) {
            head = newNode;
        }
        else {
            Node* temp = head;
            while (temp->next != nullptr) {
                temp = temp->next;
            }
            temp->next = newNode;
        }
    }

    void display() {
        Node* temp = head;
        while (temp != nullptr) {
            if (temp->coefficient == 0) {
                temp = temp->next;
                continue;
            }
            cout << "(" << temp->coefficient << "x)^" << temp->exponent;
         
            if (temp->next != nullptr) {
                cout << " + ";
            }
            
            temp = temp->next;
        }
        cout << endl;
    }

    void findRemainders(int A) {
        Node* temp = head;
        cout << "Remainders when dividing polynomial by " << A << ": \n";
        for (int i = 0; i < A; i++) {
            temp = head; // Reset temp to head for each iteration
            int result = 0;
            while (temp != nullptr) {
                if (temp->coefficient != 0) {
                    result += pow((temp->coefficient * i), temp->exponent);
                }
                temp = temp->next;
            }
            cout << "X = " << i << " - " << result % A;
            cout << endl;
        }
    }



    void readFromFile(string filename) {
        ifstream file(filename);
        if (!file) {
            cerr << "Error opening file." << endl;
            return;
        }

        int coefficient, exponent;
        while (file >> coefficient >> exponent) {
            addTerm(coefficient, exponent);
        }

        file.close();
    }
};

int main() {
    Polynomial poly;
    int A = 4;
    if (A <= 0) {
        cout << "Error A";
        return 0;
    }
    poly.readFromFile("polynomial.txt");
    cout << "Polynomial: ";
    poly.display();
    poly.findRemainders(A);
    return 0;
}
