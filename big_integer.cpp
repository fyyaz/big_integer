@@ -0,0 +1,186 @@
//BIG INTEGER
#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>
#define _DEBUG(x) cout<<#x<<": "<<x<<endl;
#define DB
#define FILE_INPUT
#define FILE_OUTPUT

using namespace std;

vector<int> to_vect(string s){
		vector<int> ret;
		for (int i = 0;i<s.length();i++)
			ret.push_back(s[i]-'0');
		return ret;
}

class big_integer{
private:
	bool zero;
	bool neg;
	vector<int> num;
	big_integer(std::vector<int> v,bool neg){
		num = v;
		this->neg = neg;
		if (v.size() == 1 && v[0] == 0)
			zero = true;
		else zero = false;
	}
public:
	big_integer(string s){
		if (s == "0")
			zero = true;
		else zero = false;
		neg = false;
		if (s[0]=='-'){
			s = s.substr(1,string::npos);
			neg = true;
		}
		num = to_vect(s);
	}
	big_integer invert(){
		this->neg = !this->neg;
		return big_integer(this->num, !(this->neg));
	}
	big_integer abs(){
		return big_integer(this->num,false);
	}
	friend big_integer sub (big_integer lhs, big_integer rhs){//lhs - rhs
		bool neg_res = false;
		//make sure lhs is bigger than rhs in magnitude
		#ifdef _DEBUG
		_DEBUG(lhs);
		_DEBUG(rhs);
		#endif
		if (lhs.num.size()<rhs.num.size())
			return sub(rhs,lhs);
		if (lhs.num.size() == rhs.num.size()){
			for (int i = 0;i<lhs.num.size();i++)
				if (lhs.num[i]<rhs.num[i])
					return sub(rhs,lhs);
				else if (lhs.num[i] == rhs.num[i])
					continue;
				else break;
		}
		std::vector<int> ret;
		if (lhs.isNegative()) neg_res = true;
		int i = lhs.num.size() - 1;
		int top_carry = 0, bot_carry = 0;
		for (int j = rhs.num.size() - 1;j>=0;){
			if (lhs.num[i]<rhs.num[j] + bot_carry)
				top_carry = 10;
			int digit = (top_carry + lhs.num[i]) - (rhs.num[j] + bot_carry);
			#ifdef _DEBUG
			_DEBUG(digit);
			_DEBUG(top_carry);
			_DEBUG(bot_carry);
			_DEBUG(lhs.num[i]);
			_DEBUG(rhs.num[i]);
			#endif 
			ret.insert(ret.begin(),digit);
			bot_carry = top_carry/10;
			top_carry = 0;
			i--;
			j--;
		}
		while(i>=0){
			if (lhs.num[i]<bot_carry)
				top_carry = 10;
			int digit = lhs.num[i] + top_carry - bot_carry;
			bot_carry = top_carry/10;;
			top_carry = 0;
			ret.insert(ret.begin(),digit);
			i--;
		}
		return big_integer(ret,neg_res);
		return big_integer("-1");
	}
	friend big_integer operator +(big_integer lhs, big_integer rhs){
		if (lhs.isNegative() && !rhs.isNegative())
			return sub(rhs,lhs);
		if (!lhs.isNegative() && rhs.isNegative())
			return sub(lhs,rhs);
		if (lhs.num.size()<rhs.num.size()){
			vector<int> t = lhs.num;
			lhs.num = rhs.num;
			rhs.num = t;
		}
		vector<int> ret;
		int carry = 0;
		int i = lhs.num.size()-1;
		for (int j = rhs.num.size()-1;j>=0;){
			int digit = lhs.num[i] + rhs.num[j] + carry;
			ret.insert(ret.begin(),digit%10);
			carry = digit/10;
			i--;
			j--;
		}
		while(i>=0){
			int digit = lhs.num[i] + carry;
			carry = digit/10;
			digit = digit%10;
			ret.insert(ret.begin(), digit);
			i--;
		}
		if (carry)
			ret.insert(ret.begin(),carry);
		if (lhs.isNegative() && rhs.isNegative())
			return big_integer(ret, true);
		return big_integer(ret,false);
	}
	bool isNegative(){
		return neg;
	}
	friend ostream& operator <<(ostream& out, big_integer b){
		if (b.isNegative())
			out<<'-';
		int printed = 0;
		for (int i = 0;i<b.num.size();i++){
			if (b.num[i] != 0){
				printed++;
				out<<b.num[i];
			}
			else{
				if (printed){
					out<<b.num[i];
					printed++;
				}
			}
		}
		if (printed == 0)
			out<<'0';
		return out;
	}
	friend istream& operator >>(istream& in, big_integer& b){
		string s;
		in>>s;
		cout<<s;
		b.num = to_vect(s);
	}  
};

int main(){
	#ifdef DB
	cout<<boolalpha;
	#endif
	#ifdef FILE_INPUT
	freopen("in.txt","r",stdin);
	#endif
	#ifdef FILE_OUTPUT
	freopen("out.txt","w",stdout);
	#endif
	string s,t;
	cin>>s>>t;
	big_integer a(s),b(t);
	cout<<a+b;
	#ifdef FILE_OUTPUT
	fclose(stdout);
	#endif
	return 0;
}
