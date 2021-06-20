
#ifndef TABLE
#define TABLE

#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>



class Table
{
public:
	Table(std::string keyColumn, const std::vector<std::string>& columns);
	~Table();
	bool good() const;
	bool insert(const std::string& recordString);
	void find(std::string key, std::vector<std::vector<std::string>>& records) const;
	int select(std::string query, std::vector<std::vector<std::string>>& records) const;

	// We prevent a Table object from being copied or assigned by
	// making the copy constructor and assignment operator unavailable.
	Table(const Table&) = delete;
	Table& operator=(const Table&) = delete;
private:
    std::vector <std::vector<std::string>> tables;
    std::string key;
	int colPosition;
    
	struct Node
	{
		std::vector <int> pos;
		std::string key_value;
		Node* left = nullptr;
		Node* right = nullptr;

	};

	Node* root;

	void insertNode(std::string x, int position);
	std::string getOperator(std::string x) const;
	void dfs(Node*ptr, std::vector<int>& pos, std::string Operator, std::string key);
	
	bool stringToDouble(std::string s, double& d) const
	{
		char* end;
		d = std::strtof(s.c_str(), &end);
		return end == s.c_str() + s.size() && !s.empty();
	}

	bool checktwoStringOk(std::string x, std::string y) const
	{
		double x1, y1;

		if (stringToDouble(x, x1) && !stringToDouble(y, y1))
		{
			return false;
		}
		return true;
	}

	
};







#endif