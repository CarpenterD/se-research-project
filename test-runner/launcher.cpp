#include <iostream>
#include <string>

using namespace std;
 
// Base class
class Marker {
    public:
        // pure virtual function providing interface framework.
        virtual int mark() = 0;
        virtual string getFeedback() = 0;

        void setFile(string f) {
            file = f;
        }

        void setOptions(string o) {
            options = o;
        }

    private:
        string file;
        string options;
};
 
// Derived classes
class VariableNameMarker: public Marker {
    public:
        int mark() { 
            return 75; 
        }

        string getFeedback(){
            return "Marking variable names";
        }
};

class IndentationChecker: public Marker {
    public:
        int mark() { 
            return 100;
        }

        string getFeedback(){
            return "Marking indentation";
        }
};
 
int main(void) {
    VariableNameMarker markVars;
    IndentationChecker  checkIndents;

    cout << "Variable Names: " << markVars.mark() << "%\n\"" + markVars.getFeedback() + "\"\n" << endl;
    cout << "Indentation: " << checkIndents.mark() << "%\n\"" + checkIndents.getFeedback() + "\"\n" << endl;

    return 0;
}