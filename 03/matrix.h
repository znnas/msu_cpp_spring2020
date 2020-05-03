#pragma once
using namespace std;

//Matrix n*m, n-rows, m-cols
class Matrix
{
private:
    // single row class
    class one_row {

    public:
        one_row(size_t cols){
            row=new int[cols];
            //row = (int*) malloc(cols * sizeof(int));
            m_col=cols;
        }

        ~one_row(){
            delete[] row;
            //free(row);
        }

        size_t get_m_col(){
        	return m_col;
        }

        const int& operator[](size_t i) const {
            if ((m_col <= i)||(i < 0)) throw out_of_range("in");
            return row[i];
        }

        int& operator[](size_t i) {
            if ((m_col <= i)||(i < 0)) throw out_of_range("const in");
            return row[i];
        }

        bool operator==(const one_row& another) const{
    		//if (this == &another) return true;
        	for(int i=0;i<m_col;i++){
        		if(row[i]!=another[i]){
        			return false;
        		}
        	}
        	return true;
        }

        bool operator!=(const one_row& another) const{
        	 return !(*this == another);
        }

    private:
       	int* row;
       	size_t m_col;
    };

    one_row* matr;
    size_t n_row;

public:
    Matrix(size_t rows, size_t cols){
        matr=static_cast<one_row*>( operator new[](rows * sizeof(one_row(cols))) );
        //matr = (one_row*) malloc(rows*sizeof(one_row(cols)));
        for (int i=0;i<rows;i++) {
            new (matr+i) one_row(cols); /*
            ¬ первоначальных попытках создавалась структура, у которой m[i] всегда совпадает в пам€ти с m[j].
            ’очетс€, чтобы во второй лекции про пам€ть нам рассказвали про placement syntax
            */
        }
        n_row=rows;
    }

    ~Matrix(){
        for (int i=0; i<n_row;i++) {
            matr[i].~one_row();
        }
        operator delete[](matr);
    }

    size_t getRows() const{
    	return n_row;
    }

    size_t getColumns() const{
        if (n_row>0) {
            return matr[0].get_m_col();
        }
        else {
            return 0;
        }
    }

    const one_row& operator[](size_t i) const{
        if ((n_row <= i)||(i < 0)) throw out_of_range("row const");
        return matr[i];
    }

    one_row& operator[](size_t i){
        if ((n_row <= i)||(i < 0)) throw out_of_range("row");
        return matr[i];
    }

    Matrix& operator*=(const int& k){
        size_t cols=getColumns();
        for(int i=0;i<n_row;i++){
            for(int j=0;j<cols;j++){
            	matr[i][j] *= k;
            }
        }
        return *this;
    }

    bool operator==(const Matrix& another) const{
		if (this == &another) return true;
		if (n_row != another.getRows()) return false;
		size_t cols=getColumns();
		if (cols != another.getColumns()) return false;
    	for(int i=0;i<n_row;i++){
    		if(matr[i]!=another[i]){
    			return false;
    		}
    	}
    	return true;
    }

    bool operator!=(const Matrix& another) const{
    	 return !(*this == another);
    }

};
