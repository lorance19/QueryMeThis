#include "Table.h"


class StringParser
{
public:
    StringParser(std::string text = "")
    {
        setString(text);
    }

    void setString(std::string text)
    {
        m_text = text;
        m_start = 0;
    }

    bool getNextField(std::string& fieldText) {
        m_start = m_text.find_first_not_of(" \t\r\n", m_start);
        if (m_start == std::string::npos)
        {
            m_start = m_text.size();
            fieldText = "";
            return false;
        }
        if (m_text[m_start] != '\'')
        {
            size_t end = m_text.find_first_of(" \t\r\n", m_start + 1);
            fieldText = m_text.substr(m_start, end - m_start);
            m_start = end;
            return true;
        }
        fieldText = "";
        for (;;)
        {
            m_start++;
            size_t end = m_text.find('\'', m_start);
            fieldText += m_text.substr(m_start, end - m_start);
            m_start = (end != std::string::npos ? end + 1 : m_text.size());
            if (m_start == m_text.size() || m_text[m_start] != '\'')
                break;
            fieldText += '\'';
        }
        return true;

    }

private:
    std::string m_text;
    size_t m_start;
};

Table::Table(std::string keyColumn, const std::vector<std::string>& columns)
{

	
	if (columns.size() > 0)
	{
		for (int i = 0; i < columns.size(); i++)
		{
			if (keyColumn == columns[i])
			{
                colPosition = i;
				key = keyColumn;
				break;
			}
		}

		tables.push_back(columns);
	}

    if (good())
    {
        root = new Node;
       
    }

}

bool Table::good() const
{
	if (tables.size() > 0) return true;
	return false;
	
}

std::string Table::getOperator(std::string sentence) const
{
    StringParser parser(sentence);
    std::string s = "";

    parser.getNextField(s);
    bool isLegitCol = false;


    for (int i = 0; i < tables[0].size(); i++)
    {
        if (tables[0][i] == s)
        {
            isLegitCol = true;
            break;
        }
    }

    if (!isLegitCol)
        return "not cols";


    parser.getNextField(s);

    if (s == "==" || s == "=" || s == "eq" || s == "EQ" || s == "eQ" || s == "Eq")
        return "==";
    else if (s == "<" || s == "lt" || s == "LT" || s == "Lt" || s == "lT")
        return "<";
    else if (s == ">" || s == "gt" || s == "GT" || s == "Gt" || s == "gT")
        return ">";
    else if (s == "!=" || s == "ne" || s == "NE" || s == "Ne" || s == "nE")
        return "!=";
    else if (s == "<=" || s == "le" || s == "LE" || s == "Le" || s == "le")
        return "<=";
    else if (s == ">=" || s == "ge" || s == "GE" || s == "Ge" || s == "gE")
        return ">=";
    else
        return "not operator";

    
}
void Table::insertNode(std::string x, int position)
{
    if (root->pos.empty())
    {
        root->pos.push_back(position);
        root->key_value = x;
        return;
    }

    Node* temp = root; 
    
    while (true)
    {   
        if (x == temp->key_value) {
            temp->pos.push_back(position);
            break;
        }
        else if (x > temp->key_value)
        {
            if (temp->right == nullptr)
            {
                temp->right = new Node;
                temp = temp->right;
                temp->pos.push_back(position);
                temp->key_value = x;
            }
            else
            {
                temp = temp->right;
                continue;
            }
        }
        else
        {
            if (temp->left == nullptr)
            {
                temp->left = new Node;
                temp = temp->left;
                temp->pos.push_back(position);
                temp->key_value = x;
            }
            else
            {
                temp = temp->left;
                continue;
            }
        }

    }

}




bool Table::insert(const std::string& recordString)
{
    std::vector<std::string> row;
    std::string s;
    StringParser parser(recordString);

    while (parser.getNextField(s))
    {
        row.push_back(s);
    }

    if (row.size() != tables[0].size())
        return false;

    tables.push_back(row);
    insertNode(row[colPosition], tables.size()-1);

	return true;
}
//
//void Table::dfs(Node* ptr ,std::vector<int>& pos, std::string Operator, std::string key)
//{
//    if (ptr == nullptr)
//        return;
//
//    if (Operator == "==")
//    {
//        
//        if (ptr->key_value == key)
//        {
//            pos = ptr->pos;
//            return;
//        }
//       
//        dfs(ptr->right, pos, Operator, key);
//        dfs(ptr->left, pos, Operator, key);
//    }
//
//    else if (Operator == "!=")
//    {
//        if (ptr->key_value != key)
//        {
//            for (int i = 0; i < ptr->pos.size(); i++)
//            {
//                pos.push_back(ptr->pos[i]);
//            }
//        }
//
//        dfs(ptr->right, pos, Operator, key);
//        dfs(ptr->left, pos, Operator, key);
//    }
//
//    else if (Operator == ">")
//    {
//        if (ptr->key_value <= key)
//        {
//            dfs(ptr->right, pos, Operator, key);
//        }
//
//        else
//        {
//            for (int i = 0; i < ptr->pos.size(); i++)
//            {
//                pos.push_back(ptr->pos[i]);
//            }
//
//            dfs(ptr->right, pos, Operator, key);
//            dfs(ptr->left, pos, Operator, key);
//        }
//    }
//    else if (Operator == "<")
//    {
//        if (ptr->key_value >= key)
//        {
//            dfs(ptr->left, pos, Operator, key);
//        }
//
//        else
//        {
//            for (int i = 0; i < ptr->pos.size(); i++)
//            {
//                pos.push_back(ptr->pos[i]);
//            }
//
//            dfs(ptr->right, pos, Operator, key);
//            dfs(ptr->left, pos, Operator, key);
//        }
//    }
//    else if (Operator == ">=")
//    {
//        if (ptr->key_value < key)
//        {
//            dfs(ptr->right, pos, Operator, key);
//        }
//
//        else
//        {
//            for (int i = 0; i < ptr->pos.size(); i++)
//            {
//                pos.push_back(ptr->pos[i]);
//            }
//
//            dfs(ptr->right, pos, Operator, key);
//            dfs(ptr->left, pos, Operator, key);
//        }
//    }
//    else 
//    {
//        if (ptr->key_value > key)
//        {
//            dfs(ptr->left, pos, Operator, key);
//        }
//
//        else
//        {
//            for (int i = 0; i < ptr->pos.size(); i++)
//            {
//                pos.push_back(ptr->pos[i]);
//            }
//
//            dfs(ptr->right, pos, Operator, key);
//            dfs(ptr->left, pos, Operator, key);
//        }
//    }
//    
//}

void Table::find(std::string key, std::vector<std::vector<std::string>>& records) const
{
    Node* temp = root; 
    
    while (true)
    {
        if (temp == nullptr)
            break;

        if (temp->key_value == key)
        {
            for (int i = 0; i < temp->pos.size(); i++)
            {
                records.push_back(tables[temp->pos[i]]);
            }

            break;
        }
        else if (temp->key_value > key)
        {
            temp = temp->left;
            continue;
        }
        else 
        {
            temp = temp->right;
            continue;
        }
        
    }

	return;
}
int Table::select(std::string query, std::vector<std::vector<std::string>>& records) const
{

    records.clear();

    std::string Operator = getOperator(query);
    
    if (Operator == "not operator" || Operator == "not cols")
        return -1;
    
    
    int corruptCount = 0;


    int colsPos = -1;

    StringParser parser(query);
    std::string s = "";
    

    parser.getNextField(s);

    for (int i = 0; i < tables[0].size(); i++)
    {
        if (tables[0][i] == s)
        {
            colsPos = i;
        }
    }

    double val1, val2;

    parser.getNextField(s);
    parser.getNextField(s);

    if (Operator == "==")
    {
        for (int i = 1; i < tables.size(); i++)
        {
            if (tables[i][colsPos] == s)
                records.push_back(tables[i]);
        }
    }
    else if (Operator=="<")
    {
        for (int i = 1; i < tables.size(); i++)
        {

            if (isdigit(tables[i][colsPos][0]) && !isdigit(s[0]))
                return -1;



            if (stringToDouble(tables[i][colsPos], val1) &&
                stringToDouble(s, val2))
            {
                if (val1 < val2)
                {
                    records.push_back(tables[i]);
                }
            }
            else if (!stringToDouble(tables[i][colsPos], val1) &&
                     !stringToDouble(s, val2))
            {
                if (tables[i][colsPos] < s) 
                {
                    records.push_back(tables[i]);

                }
            }
            
            else if (!stringToDouble(tables[i][colsPos], val1) &&
                stringToDouble(s, val2))
            {
                corruptCount++;
            }
                
            
        }
    }
    else if (Operator == "<=")
    {
        for (int i = 1; i < tables.size(); i++)
        {
            if (isdigit(tables[i][colsPos][0]) && !isdigit(s[0]))
                return -1;



            if (stringToDouble(tables[i][colsPos], val1) &&
                stringToDouble(s, val2))
            {
                if (val1 <= val2)
                {
                    records.push_back(tables[i]);
                }
            }
            else if (!stringToDouble(tables[i][colsPos], val1) &&
                !stringToDouble(s, val2))
            {
                if (tables[i][colsPos] <= s)
                {
                    records.push_back(tables[i]);

                }
            }

            else if (!stringToDouble(tables[i][colsPos], val1) &&
                stringToDouble(s, val2))
            {
                corruptCount++;
            }

        }
    }
    else if (Operator == ">")
    {
        for (int i = 1; i < tables.size(); i++)
        {
            if (isdigit(tables[i][colsPos][0]) && !isdigit(s[0]))
                return -1;



            if (stringToDouble(tables[i][colsPos], val1) &&
                stringToDouble(s, val2))
            {
                if (val1 > val2)
                {
                    records.push_back(tables[i]);
                }
            }
            else if (!stringToDouble(tables[i][colsPos], val1) &&
                !stringToDouble(s, val2))
            {
                if (tables[i][colsPos] > s)
                {
                    records.push_back(tables[i]);

                }
            }

            else if (!stringToDouble(tables[i][colsPos], val1) &&
                stringToDouble(s, val2))
            {
                corruptCount++;
            }

        }
    }
    else if (Operator == ">=")
    {
        for (int i = 1; i < tables.size(); i++)
        {
            if (isdigit(tables[i][colsPos][0]) && !isdigit(s[0]))
                return -1;



            if (stringToDouble(tables[i][colsPos], val1) &&
                stringToDouble(s, val2))
            {
                if (val1 >= val2)
                {
                    records.push_back(tables[i]);
                }
            }
            else if (!stringToDouble(tables[i][colsPos], val1) &&
                !stringToDouble(s, val2))
            {
                if (tables[i][colsPos] >= s)
                {
                    records.push_back(tables[i]);

                }
            }

            else if (!stringToDouble(tables[i][colsPos], val1) &&
                stringToDouble(s, val2))
            {
                corruptCount++;
            }

        }
    }
    else if (Operator == "!=")
    {
        for (int i = 1; i < tables.size(); i++)
        {
            if (tables[i][colsPos] != s)
                records.push_back(tables[i]);
        }
    }


    if(records.size()==0)
        return -1;

	return corruptCount;
}
Table::~Table()
{

}