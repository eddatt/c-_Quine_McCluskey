#include<iostream>
#include<stack>
#include<vector>
#include<string>

using namespace std;

bool is_not(char c){ return c == '~'; }
bool is_and(char c){ return c == '&'; }
bool is_xor(char c){ return c == '^'; }
bool is_orr(char c){ return c == '|'; }
bool is_bac(char c){ return c == '('; }
bool is_oper(char c){ return is_not(c) || is_and(c) || is_xor(c) || is_orr(c) || is_bac(c); }

bool is_operand(char c){
	return c >= 'A' &&c <= 'H';
}

int si_prior(char c){
	if (c == '~') return 5;
	else if (c == '&') return 4;
	else if (c == '^') return 3;
	else if (c == '|') return 2;
	else if (c == '(') return 1;
	else return 0;
}

int re_prior(char c, char d){
	if (is_bac(d)) return 0;
	else if (si_prior(c) >= si_prior(d)) return 1;
	else return 0;
}

void print_stack(stack<char> s){
	stack<char> rs = s;
	vector<char> vs;
	while (rs.size()){
		vs.push_back(rs.top());
		rs.pop();
	}
	cout << "Stack:  ";
	for (int i = vs.size() - 1; i >= 0; i--){
		cout << vs[i] << "  ";
	}
	cout << endl;
}

void print_bstack(stack<bool> s){
	stack<bool> rs = s;
	vector<bool> vs;
	while (rs.size()){
		vs.push_back(rs.top());
		rs.pop();
	}
	cout << "Stack:  ";
	for (int i = vs.size() - 1; i >= 0; i--){
		cout << vs[i] << "  ";
	}
	cout << endl;
}

string shorten(string m){
	stack<char> s;
	string sur;
	int i;

	for (i = 0; i < m.size(); i++){
		if (is_operand(m[i])) { sur += m[i]; }
		else if (is_oper(m[i])){
			if (is_not(m[i + 1])){
				while (s.size() && re_prior(s.top(), m[i])){
					sur += s.top();
					s.pop();
				}
				s.push(m[i]);
				s.push(m[i + 1]);
				i++;
			}
			else{
				while (s.size() && re_prior(s.top(), m[i])){
					sur += s.top();
					s.pop();
				}
				s.push(m[i]);
			}

		}
		else if (m[i] == ')'){
			while (s.top() != '('){
				sur += s.top();
				s.pop();
			}
			s.pop();
		}
		else {
			s.push(m[i]);
		}
	}
	if (s.size()){
		while (s.size()){
			sur += s.top();
			s.pop();
		}
	}
	return sur;
}

bool alpha_num(char c, vector<bool> v){
	return v[7 - (c - 'A')];
}

bool char_oper(char c, bool left, bool right){
	if (c == '&'){ return left&&right; }
	else if (c == '^'){ return left^right; }
	else if (c == '|'){ return left || right; }
	else return 0;
}

void set_bin(int i, vector<bool> &v){
	v[7] = i % 2;
	for (int j = 6; j >= 0; j--){
		i /= 2;
		v[j] = i % 2;
	}
}

void vec_print(vector<bool> v){
	cout << "Vec:  ";
	for (int i = 0; i < v.size(); i++){
		cout << v[i] << ' ';
	}
	cout << endl;
}

bool str_to_bool(string str, int count){
	bool temp;
	bool result;
	vector<bool> num;
	num.push_back(0);
	num.push_back(0);
	num.push_back(0);
	num.push_back(0);
	num.push_back(0);
	num.push_back(0);
	num.push_back(0);
	num.push_back(0);
	set_bin(count, num);
	stack<bool> s;
	for (int i = 0; i < str.size(); i++){
		if (is_operand(str[i])){
			s.push(alpha_num(str[i], num));
		}
		else if (is_not(str[i]) && s.size()>0){
			bool res = s.top();
			res = !res;
			s.pop();
			s.push(res);
		}
		else if (is_oper(str[i]) && s.size()>1 && !is_not(str[i])){
			bool left = s.top();
			s.pop();
			bool right = s.top();
			s.pop();
			temp = char_oper(str[i], left, right);
			s.push(temp);
		}
	}
	result = s.top();
	return result;
}

int two_power(int k){
	int re = 1;
	while (k){
		re *= 2;
		k--;
	}
	return re;
}

int num_reverse(int a, int count){
	vector<int> v;
	int b = a;
	int re = 0;
	while (b){
		v.push_back(count % 2);
		count /= 2;
		b--;
	}
	for (int j = 0; j < a; j++){
		re += (two_power(a - 1 - j))*v[j];
	}
	return re;
}

int is_in_vec(char c, vector<char> v){
	if (v.size() == 0) return 0;
	for (int i = 0; i < v.size(); i++){
		if (c == v[i]) return 1;
	}
	return 0;
}

int oper_count(string x){
	vector<char> c;
	if (x.size() == 0) return 0;
	for (int i = 0; i < x.size(); i++){
		if (is_operand(x[i]) && is_in_vec(x[i], c) == 0){
			c.push_back(x[i]);
		}
	}
	return c.size();
}

char int_to_char(int x){
	if (x == 1) return '1';
	else return '0';
}

string expr_to_truthtable(int n, const string strx){
	int result;
	int count = 0;
	int oper_num = 0;
	string x = strx;
	oper_num = oper_count(x);
	if (n != oper_num ||n>8){
		cout << "Invlid input!!!" << endl;
		return "";
	}
	string sur = shorten(x);
	string rr;
	cout << "The result truthtable:" << endl;
	for (count = 0; count < two_power(oper_num); count++){
		result = str_to_bool(sur, num_reverse(oper_num, count));
		rr += int_to_char(result);
	}
	return rr;
}

int is_valid_truthtable(string str){
	if (str.size() == 0) return 0;
	int count = 0;
	if (str.size() == 2 || str.size() == 4 || str.size() == 8 || str.size() == 16 || str.size() == 32 || str.size() == 64 || str.size() == 128 || str.size() == 256){
		for (int i = 0; i < str.size(); i++){
			if (str[i] == '0' || str[i] == '1' || str[i] == 'x') count++;
		}
		return count == str.size();
	}
	return 0;
}

int operand_num(string str){
	int i = 0;
	int j = str.size();
	while (j){
		i++;
		j /= 2;
	}
	return i - 1;
}

string num_to_str(int i, int n){
	string m;
	int l = n - 1;
	for (int j = 0; j < n; j++){
		m += char(int('A') + j);
		if (i / (two_power(l)))
			m += ' ';
		else
			m += '~';
		i -= (i / two_power(l)) *(two_power(l));
		l--;
	}
	return m;
}

vector<pair<string, int>> read_truthtable(string str){
	vector<pair<string, int>> v;
	int n = operand_num(str);
	for (int i = 0; i < str.size(); i++){
		if (str[i] == '1'){
			pair<string, int> p;
			p.first = num_to_str(i, n);
			p.second = 0;
			v.push_back(p);
		}
	}
	return v;
}

void vec_pair_print(vector<pair<string, int>> v){
	cout << "This is vec_pair_print:  " << endl;
	for (int i = 0; i < v.size(); i++){
		cout << "I.first:  " << v[i].first << "    I.second:  " << v[i].second << endl;
	}
}

string logic_ex_print(vector<string> v){
	if (v.size() == 0) return "";
	int i;
	string m;
	for (i = 0; i < v.size() - 1; i++){
		m += v[i];
		m += "+";
	}
	m += v[i];
	string re;
	for (int j = 0; j < m.size(); j++){
		if (m[j] != ' ') re += m[j];
	}
	return re;
}

int is_near(string a, string b){
	if (a.size() != b.size()) return 0;
	int i = 0;
	for (int j = 0; j < a.size(); j++){
		i += (a[j] != b[j]);
	}
	if (i == 1) return 1;
	else return 0;
}

string near_reduce(string a, string b){
	string m = a;
	for (int i = 0; i < a.size(); i++){
		if (a[i] != b[i]) {
			m[i - 1] = ' ';
			m[i] = ' ';
		}
	}
	return m;
}

int vec_near_mark(vector<pair<string, int>> &v){
	if (v.size() == 0) return 0;
	if (v.size() == 1) return 0;
	for (int i = 0; i < v.size(); i++){
		for (int j = 0; j < v.size(); j++){
			if (is_near(v[i].first, v[j].first)) {
				v[i].second = 1;
				v[j].second = 1;
			}
		}
	}
	return 1;
}

int is_vec_seperate(vector<pair<string, int>> v){
	int re = 1;
	for (int i = 0; i < v.size(); i++){
		if (v[i].second == 1)  re = 0;
	}
	return re;
}

vector<string> vec_pair_tovec(vector<pair<string, int>> v){
	vector<string> x;
	for (int i = 0; i < v.size(); i++){
		x.push_back(v[i].first);
	}
	return x;
}

int one_num(int i){
	int result = 0;
	while (i){
		if (i % 2) {
			result++;
			i /= 2;
		}
		else
			i /= 2;
	}
	return result;
}

int string_to_onenum(string s){
	int is_not = 0;
	int is_opr = 0;
	for (int i = 0; i < s.size(); i++){
		if (is_operand(s[i])) is_opr++;
		if (s[i] == '~') is_not++;
	}
	return is_opr - is_not;
}

int is_void_vec(vector<pair<string, int>> a){
	if (a.size() == 0) return 1;
}

int is_in_vec(string s, vector<pair<string, int>> a){
	if (a.size() == 0) return 0;
	for (int i = 0; i < a.size(); i++){
		if (a[i].first == s) return 1;
	}
	return 0;
}

int vec_no_mark(vector<pair<string, int>> a){
	if (a.size() == 0) return 1;
	for (int i = 0; i < a.size(); i++){
		if (a[i].second != 0) return 0;
	}
	return 1;
}

int vec_reset(vector<pair<string, int>> a){
	if (a.size() == 0) return 0;
	for (int i = 0; i < a.size(); i++){
		a[i].second = 0;
	}
	return 1;
}

int is_in(string a, string b){
	if (a.size() != b.size()) return 0;
	int same = 1;
	for (int i = 0; i < a.size(); i += 2){
		if (a[i] == b[i] && a[i + 1] == b[i + 1]) continue;
		else if (a[i] != b[i] && is_operand(a[i]) && b[i] == ' ') continue;
		else same = 0;
	}
	return same;
}

string the_uninclude(string s, vector<pair<string, int>> a){
	if (a.size() == 0) return "";
	string str;
	for (int i = 0; i < a.size(); i++){
		if (a[i].second == 0){
			if (is_in(a[i].first, s)) str += '1';
			else str += '0';
		}
	}
	return str;
}

int is_inclu(string a, string b){
	if (a.size() != b.size()) return 0;
	int inclu = 1;
	for (int i = 0; i < a.size(); i++){
		if (a[i] == b[i]) continue;
		else if (a[i] != b[i] && b[i] == '1') continue;
		else inclu = 0;
	}
	return inclu;
}

int is_all_inclu(vector<pair<string, int>> a){
	int num = 0;
	for (int i = 0; i < a.size(); i++) {
		if (a[i].second == 2) num++;
	}
	return num == a.size();
}

int vec_find_string(string s, vector<pair<string, int>> a){
	if (a.size() == 0) return -1;
	for (int i = 0; i < a.size(); i++){
		if (s == a[i].first) return i;
	}
	return -1;
}

string vec_to_exp(vector<pair<string, int>> v){
	if (v.size() == 0) return "";
	string re;
	string rr;
	for (int i = 0; i < v.size(); i++){
		if (v[i].second == 2){
			re += v[i].first;
			re += "+";
		}
	}
	for (int j = 0; j < re.size() - 1; j++){
		if (re[j] != ' ') rr += re[j];
	}
	return rr;
}

string main_loop(string exp){
	int is_ini_seperate;
	int sep_loop_mark;
	int oper_num;
	string in = exp;
	oper_num = operand_num(in);
	vector<pair<string, int>> a = read_truthtable(in);
	vector<pair<string, int>> logic_ex_ori = a;
	vector<pair<string, int>> allogic_ex_store = a;
	vector<pair<string, int>> not_near_ex_stroe;
	vector<pair<string, int>> logic_pipeline = a;
	vector<string> result;
	vector<pair<string, int>> process[9][10];
	int process_rowindex = 0;
	int process_colindex = 0;
	vec_near_mark(a);
	is_ini_seperate = is_vec_seperate(a);
	sep_loop_mark = is_vec_seperate(a);
	if (is_ini_seperate) {
		return logic_ex_print(vec_pair_tovec(a));
	}

	for (int ia = 0; ia < logic_pipeline.size(); ia++){
		int true_num = string_to_onenum(logic_pipeline[ia].first);
		process[true_num][process_colindex].push_back(logic_pipeline[ia]);
	}

	while (!sep_loop_mark){
		logic_pipeline.clear();
		for (int ia = 0; ia < 9; ia++){
			if (process[ia][process_colindex].size()){
				for (int iaa = 0; iaa < process[ia][process_colindex].size(); iaa++){
					logic_pipeline.push_back(process[ia][process_colindex][iaa]);
				}
			}
		}
		vec_near_mark(logic_pipeline);
		//导入值
		for (int iab = 0; iab < 9; iab++){
			if (process[iab][process_colindex].size()){
				for (int iac = 0; iac < process[iab][process_colindex].size(); iac++){
					int iad = vec_find_string(process[iab][process_colindex][iac].first, logic_pipeline);
					process[iab][process_colindex][iac].second = logic_pipeline[iad].second;
				}
			}
		}
		int vec_all_nomark = vec_no_mark(logic_pipeline);
		//如果出现临近项标记
		//vec内都是不临近的
		if (vec_all_nomark == 1){
			sep_loop_mark = 0;
			break;
		}
		//合并
		else{
			for (int id = 8; id > 0; id--){
				if (process[id][process_colindex].size() == 0 || process[id - 1][process_colindex].size() == 0) continue;
				else{
					for (int ie = 0; ie < process[id][process_colindex].size(); ie++){
						for (int ig = 0; ig < process[id - 1][process_colindex].size(); ig++){
							if (is_near(process[id][process_colindex][ie].first, process[id - 1][process_colindex][ig].first)){
								string cox = near_reduce(process[id][process_colindex][ie].first, process[id - 1][process_colindex][ig].first);
								int index = string_to_onenum(cox);
								pair<string, int> newp;
								newp.first = cox;
								newp.second = 0;
								if (!is_in_vec(cox, process[index][process_colindex + 1])){
									process[index][process_colindex + 1].push_back(newp);
								}
							}
						}
					}
				}
			}
		}
		//下一列
		process_colindex++;
	}
	for (int ih = 0; ih < 9; ih++){
		for (int ii = 0; ii < 10; ii++){
			if (process[ih][ii].size() != 0) {
				for (int ij = 0; ij < process[ih][ii].size(); ij++){
					if (process[ih][ii][ij].second == 0) not_near_ex_stroe.push_back(process[ih][ii][ij]);
				}
			}
		}
	}

	int all_include = 0;
	while (!all_include){
		//去重
		for (int ik = 0; ik < not_near_ex_stroe.size() - 1; ik++){
			for (int il = ik + 1; il < not_near_ex_stroe.size(); il++){
				if (not_near_ex_stroe[ik].second == 0 && not_near_ex_stroe[il].second == 0){
					string s1 = the_uninclude(not_near_ex_stroe[ik].first, logic_ex_ori);
					string s2 = the_uninclude(not_near_ex_stroe[il].first, logic_ex_ori);
					if (is_inclu(s1, s2)){
						not_near_ex_stroe[ik].second = -1;
					}
				}
			}
		}
		//仅包
		int if_onlyone = 0;
		for (int im = 0; im < logic_ex_ori.size(); im++){
			int inclu_num = 0;
			int in_store;
			for (int in = 0; in < not_near_ex_stroe.size(); in++){
				if (logic_ex_ori[im].second == 0 && not_near_ex_stroe[in].second == 0){
					if (is_in(logic_ex_ori[im].first, not_near_ex_stroe[in].first)) {
						inclu_num++;
						in_store = in;
					}
				}
			}
			if (inclu_num == 1){
				logic_ex_ori[im].second = 2;
				not_near_ex_stroe[in_store].second = 2;
				if_onlyone++;
			}
		}
		//由仅包选择的式，反选基础项
		for (int io = 0; io < not_near_ex_stroe.size(); io++){
			for (int ip = 0; ip < logic_ex_ori.size(); ip++){
				if (is_in(logic_ex_ori[ip].first, not_near_ex_stroe[io].first) && not_near_ex_stroe[io].second == 2){
					logic_ex_ori[ip].second = 2;
				}
			}
		}
		//没有仅包含操作，随机选
		all_include = is_all_inclu(logic_ex_ori);
		if (if_onlyone == 0 && all_include == 0){
			while (!all_include){
				for (int iq = 0; iq < logic_ex_ori.size(); iq++){
					//没选过的基础项
					if (logic_ex_ori[iq].second == 0){
						for (int ir = 0; ir < not_near_ex_stroe.size(); ir++){
							//找到满足的第一个式子
							if (is_in(logic_ex_ori[iq].first, not_near_ex_stroe[ir].first)){
								not_near_ex_stroe[ir].second = 2;
								logic_ex_ori[iq].second = 2;
								//反选
								for (int is = 0; is < logic_ex_ori.size(); is++){
									if (is_in(logic_ex_ori[is].first, not_near_ex_stroe[ir].first)){
										logic_ex_ori[is].second = 2;
									}
								}
							}
						}
					}
				}
				all_include = is_all_inclu(logic_ex_ori);
			}
		}
		all_include = is_all_inclu(logic_ex_ori);
	}
	string ree = vec_to_exp(not_near_ex_stroe);
	return ree;
}

bool is_num(string str){
	if (str.size() == 0) return 0;
	int is_num = 0;
	for (int i = 0; i < str.size(); i++){
		if (str[i] == '1' || str[i] == '0') is_num++;
	}
	return is_num == str.size();
}

bool all_num(stack<string> s){
	if (s.size() == 0)  return 0;
	int num = s.size();
	int count = 0;
	while (s.size()){
		if (is_num(s.top())) count++;
		count++;
		s.pop();
	}
	return count == num;
}

string x_to_one(string s){
	string k = s;
	if (s.size() == 0) return "";
	for (int i = 0; i < s.size(); i++){
		if (s[i] == 'x') {
			k[i] = '1';
			return k;
		}
	}
}

string x_to_zero(string s){
	string k = s;
	if (s.size() == 0) return "";
	for (int i = 0; i < s.size(); i++){
		if (s[i] == 'x') {
			k[i] = '0';
			return k;
		}
	}
}

vector<string> exp_sep(string x){
	vector<string> a;
	stack<string> sh;
	string temp;
	if (x.size() == 0) {
		cout << "Error cant be null" << endl;
		return a;
	}
	if (is_num(x)) return a;
	sh.push(x);
	while (sh.size() != 0){
		temp = sh.top();
		sh.pop();
		if (is_num(x_to_one(temp)) != 1){
			sh.push(x_to_one(temp));
			sh.push(x_to_zero(temp));
		}
		else{
			a.push_back(x_to_one(temp));
			a.push_back(x_to_zero(temp));
		}
	}
	return a;
}

string truthtable_to_expr(const string &strx){

	string x = strx;
	if (is_valid_truthtable(x)){
		vector<string> result;
		if (is_num(x)) {
			string once_re = main_loop(x);
			cout << "When input is:  ";
			cout << x << endl;
			cout << "Final result (the shortest) is:  "<<endl;
			//cout << once_re << endl;
			return once_re;
		}
		else{
			vector<string> xx = exp_sep(x);
			for (int i = 0; i < xx.size(); i++){
				cout << "When input is:  " << xx[i] << endl;
				string once_re = main_loop(xx[i]);
				cout << "re is:  ";
				cout << once_re << endl;
				result.push_back(once_re);
			}
		}

		int shortest = result[0].size();
		int short_index = 0;
		for (int j = 0; j < result.size(); j++){
			if (result[j].size() < shortest){
				short_index = j;
				shortest = result[j].size();
			}
		}
		cout << endl;
		cout << "Final result (the shortest) is:" << endl;
		return result[short_index];
	}
	else{
		cout << "Input is invalid, please try again!!!" << endl;
		return "";
	}

}

int valid_expr(string s){
	if (s.size() == 0) return 0;
	for (int i = 0; i < s.size(); i++){
		if (!is_operand(s[i]) || !is_oper(s[i]) || s[i] != '(' || s[i] != ')')return 0;
	}
	for (int i = 0; i < s.size() - 1; i++){
		if (is_operand(s[i]) && is_operand(s[+1])) return 0;
	}
	return 1;
}

int main(){
	string x;
	string xx;
	cout << "What you want to do?"<<endl;
	cout << "0 for expr_to_truthtable, 1 for truthtable_to_expr" << endl;
	cin >> x;
	if (x == "0"){
		cout << "Please enter a valid logic expression, rules are as follows:" << endl;
		cout << "1.operand max to ABCDEFGH, in capital, shall use in order, and A has max priority." << endl;
		cout << "2.operator follows the priority as follows (~)>(^)>(&)>(|)." << endl;
		cout << "3.can use bracket to change priority of rule No.2." << endl;
		cout << "4.when use ~, shall before the operand ex: ~A&B, but ~~A&B is not allowed." << endl;
		cout << "5.no blank is needed, and is not allowed." << endl;
		cout << "The result truthtable format ex:ABCD=0000 to ABCD=1111." << endl;
		cout << "Please enter expr" << endl;
		cin >> xx;
		int y = oper_count(xx);
		if (valid_expr(xx)){
			cout << expr_to_truthtable(y, xx);
			return 0;
		}
		else{
			cout << "Invalid input!" << endl;
			return 0;
		}

	}
	if (x == "1"){
		cout << "Please enter a truthtable, rules are as follows:" << endl;
		cout << "1.length should be amoug 2,4,8,16,32,64,128,256." << endl;
		cout << "2.can only contain 0,1,x, x for(0|1)." << endl;
		cout << "3.the truthtable starts from 0* to 1* ex: 000 to 111." << endl;
		cout << "The process will print all the result for x replaced by 0 and 1." << endl;
		cout << "Then take the shortest to be the final result." << endl;
		cout << "Please enter the truthtable" << endl;
		cin >> xx;
		string rx = truthtable_to_expr(xx);
		//if (rx == " ") rx = "1";
		cout<<rx;
		return 1;
	}
	if (x != "0" &&x != "1"){
		cout << "Nothing more can do!" << endl;
		return 0;
	}
}