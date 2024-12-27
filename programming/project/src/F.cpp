#include<iostream>
#include<vector>
#include<algorithm>
#include<fstream>
#include<cmath>

using namespace std;

class Table{
private:
    vector<vector<double>> table;
    int n=0;

    double truncated_power(const double &ti, const double &x, const int &n){
        if(ti >= x){
            return pow(ti - x, n);
        } else {
            return 0;
        }
    }

public:
    Table(const vector<double> &t, const double &x){
        n = t.size();
        table.resize(n, vector<double>(n, 0.0));

        for(int i = 0; i < n; ++i){
            table[i][0] = truncated_power(t[i], x, n - 2);
        }

        for(int i = 1; i < n; ++i){
            for(int j = i; j < n; ++j){
                table[j][i] = (table[j][i - 1] - table[j - 1][i - 1]) / (t[j] - t[j - i]);
            }
        }
    }

    vector<vector<double>> getTable(){
        return table;
    }
};

int main(){
    vector<double> t1{0.0, 1.0, 2.0};
    int n = t1.size();

    ofstream out1("F_n=1.csv");
    if (!out1.is_open()) {
        cerr << "Error opening F_n=1.csv" << endl;
        return -1;
    }

    for(double k = -1.0; k < 3.0; k += 0.01){
        Table table(t1, k);
        vector<vector<double>> T = table.getTable();
        out1 << k;

        for (int i = 0; i < n; ++i) {
            for (int j = i; j < n; ++j) {
                out1 << "," << T[j][i];
            }
        }
        out1 << endl;
    }

    out1.close();
    cout << "Data saved to F_n=1.csv" << endl;

    vector<double> t2{0.0, 0.5, 0.7, 1.0};
    int n1 = t2.size();

    ofstream out2("F_n=2.csv");
    if (!out2.is_open()) {
        cerr << "Error opening F_n=2.csv" << endl;
        return -1;
    }

    for(double k = -0.5; k < 1.5; k += 0.01){
        Table table(t2, k);
        vector<vector<double>> T2 = table.getTable();
        out2 << k;

        for (int i = 0; i < n1; ++i) {
            for (int j = i; j < n1; ++j) {
                out2 << "," << T2[j][i];
            }
        }
        out2 << endl;
    }

    out2.close();
    cout << "Data saved to F_n=2.csv" << endl;

    return 0;
}
