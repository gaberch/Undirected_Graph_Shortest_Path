/**
 * An example unit test using doctest. See documentation at
 * https://github.com/onqtam/doctest/blob/master/doc/markdown/tutorial.md
 */
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
#include <iostream>
#include <list>
#include <sstream>
#include <string>
#include <vector>

class AdjacencyMatrix
{

private:

   std::vector <int> *adjacency_matrix = nullptr;
   int vertex_number = 0;

   bool reset_matrix(int num1, int num2){
      if ((num1 == num2) || (num1 >= vertex_number || num2 >= vertex_number))
      {
         delete[] adjacency_matrix;
         adjacency_matrix = new std::vector <int> [vertex_number];
         return false;
      }
      return true;
   }

public:

   void new_matrix(int vertex)
   {
      if (adjacency_matrix != nullptr)
      {
         delete[] adjacency_matrix;
         adjacency_matrix = nullptr;
      }
      vertex_number = vertex;
      adjacency_matrix = new std::vector<int>[vertex_number];
   }

   bool add_matrix_element(int v1, int v2)
   {
      if (reset_matrix(v1,v2))
      {
         adjacency_matrix[v1].push_back(v2);
         adjacency_matrix[v2].push_back(v1);
         return true;
      }
      return false;
   }

   bool check_bounds (int v1, int v2)
   {
      return !(v1 >= vertex_number || v2 >= vertex_number);
   }

   int read_matrix(int r, int c)
   {
      return adjacency_matrix[r][c];
   }

   int matrix_row_size(int r)
   {
      return adjacency_matrix[r].size();
   }

};

class Graph
{

private:
   int vertex_number = 0;
   AdjacencyMatrix adj_matrix;

   void print_shortest_path (std::list<int> shortest_path)
   {
      unsigned i = 0;
      for (auto vertex : shortest_path)
      {
         if (i < shortest_path.size() - 1)
         {
            std::cout << vertex << "-";

         }
         else
         {
            std::cout << vertex << std::endl;
         }
         i++;
      }
   }

public:
   bool check_bounds(int v1, int v2)
   {
      return adj_matrix.check_bounds(v1, v2);
   }

   void new_matrix (int vertex)
   {
      vertex_number = vertex;
      adj_matrix.new_matrix(vertex_number);
   }

   bool add_matrix_element(int v1, int v2)
   {
      return adj_matrix.add_matrix_element(v1,v2);
   }

   bool bfs_search(int init, int final)
   {

      std::list<int> current_queue;
      std::list<int> previous_v;
      std::list<int> next_v;
      std::list<int> shortest_path;

      bool *vertex_checked;
      vertex_checked = new bool[vertex_number];

      for (int i = 0; i < vertex_number; i++)
      {
         vertex_checked[i] = false;
      }
      vertex_checked[init] = true;
      current_queue.push_back(init);

      // Breadth First Search
      while(!current_queue.empty())
      {
         int row = current_queue.front();
         current_queue.pop_front();
         for (int col = 0; col < adj_matrix.matrix_row_size(row); col++)
         {

            if (!vertex_checked[adj_matrix.read_matrix(row,col)])
            {

               next_v.push_back(adj_matrix.read_matrix(row,col));
               previous_v.push_back(row);
               current_queue.push_back(adj_matrix.read_matrix(row,col));
               vertex_checked[adj_matrix.read_matrix(row,col)] = true;
            }

            if (adj_matrix.read_matrix(row,col) == final)
            {
               int last_v = final;
               while(!next_v.empty())
               {
                  if (next_v.back() != last_v)
                  {
                     next_v.pop_back();
                     previous_v.pop_back();
                  }
                  else
                  {
                     shortest_path.push_back(next_v.back());
                     last_v = previous_v.back();
                     previous_v.pop_back();
                     next_v.pop_back();
                  }
               }
               shortest_path.push_back(init);
               shortest_path.reverse();
               print_shortest_path(shortest_path);
               while (!current_queue.empty())
               {
                  current_queue.pop_front();
               }
               delete[] vertex_checked;
               return true;
            }
         }
      }

      delete[] vertex_checked;
      return false;
   }

};

class Input_Parser
{

private:

   int flag = 0;
   Graph graph;

   void vertex_cmd (std::string input, std::size_t whitespace)
   {
      char *end;
      input = input.substr(whitespace, input.length() - 1);
      int num_vertices = strtol(input.c_str(), &end, 10);
      if (num_vertices < 2)
      {
         std::cerr << "Error: Set of vertices need to be at least 2\n";
      }
      else
      {
         graph.new_matrix(num_vertices);
         flag = 1;
      }
   }

   void edge_cmd (const std::string &input, std::size_t whitespace)
   {
      if (flag == 2)
      {
         std::cerr << "Error: Set of edges have already been captured. To enter a new set of edges, a new vertex command must be entered first" << '\n';
      }
      else if (flag == 0)
      {
         std::cerr << "Error: Number of vertices need to be inputted first\n";
      }
      else
      {
         std::string edge_set = input.substr(whitespace, input.length()-1);
         char edge_set_rb = '}';
         std::size_t current_pos;
         std::string vertex1, vertex2;
         current_pos = edge_set.find(edge_set_rb, 0);
         while (current_pos != std::string::npos)
         {
            char *end;
            std::size_t x = edge_set.find(',', 0) - 1 - edge_set.find('<', 0);
            std::size_t y = edge_set.find('>', 0) - 1 - edge_set.find(',', 0);
            vertex1 = edge_set.substr(edge_set.find('<', 0) + 1, x);
            vertex2 = edge_set.substr(edge_set.find(',', 0) + 1, y);
            int v1 = strtol(vertex1.c_str(),&end,10);
            int v2 = strtol(vertex2.c_str(),&end,10);

            if (v1 == v2)
            {
               edge_set.erase(0,edge_set.find_first_of('>', 0) + 2);
               flag = 2;
               current_pos = edge_set.find(edge_set_rb, 0);
               continue;
            }

            if (!(graph.add_matrix_element(v1,v2)))
            {
               std::cerr << "Error: Edge specified is out of bounds\n";
               flag = 0;
               break;
            }
            edge_set.erase(0,edge_set.find_first_of('>', 0) + 2);
            flag = 2;
            current_pos = edge_set.find(edge_set_rb, 0);
         }

      }
   }

   void path_cmd(std::string input, size_t whitespace)
   {
      if (flag == 2)
      {
         input = input.substr(whitespace + 1, input.length() - 1);
         std::size_t whitespace = input.find(' ', 0);
         std::string v1 = input.substr(0,whitespace);
         std::string v2 = input.substr(whitespace + 1, input.length() - 1);
         if (!v1.empty() && !v2.empty())
         {
            char *end;
            int init = strtol(v1.c_str(),&end,10);
            int final = strtol(v2.c_str(),&end,10);
            if (!graph.check_bounds(init,final))
            {
               std::cerr << "Error: Vertices specified are not in graph specified\n";
            }
            else if (init == final)
            {
               std::cout << init << "-" << final << std::endl;
            }
            else if (!graph.bfs_search(init,final))
            {
               std::cerr << "Error: Path does not exist\n";
            }
         }
      }
      else
      {
         std::cerr << "Error: Not enough information provided for graph input\n";
      }
   }

public:
   void cmd_parser(const std::string &input)
   {
      int v_input = input.find('V',0);
      int e_input = input.find('E', 0);
      int s_input = input.find('s', 0);
      int whitespace = input.find(' ', 0);

      if ((v_input != -1) && (whitespace != -1))
      {
         vertex_cmd(input, whitespace);
      }
      else if ((e_input != -1) && (whitespace != -1))
      {
         edge_cmd(input, whitespace);
      }
      else if ((s_input != -1) && (whitespace != -1))
      {
         path_cmd(input, whitespace);
      }
      else if (!input.empty())
      {
         std::cerr << "Error: Invalid command\n";
      }

   }

};


TEST_CASE("Test 1")
{
    std::ostringstream oss;
    std::streambuf* p_cout_streambuf = std::cout.rdbuf();
    std::cout.rdbuf(oss.rdbuf());
    Input_Parser Parser;

    std::string input = "V 4";
    Parser.cmd_parser(input);
    input = "E {<0,1>}";
    Parser.cmd_parser(input);
    input = "s 0 1";
    Parser.cmd_parser(input);
    std::cout.rdbuf(p_cout_streambuf);

    CHECK(oss.str() == "0-1\n");
}

TEST_CASE("Test 2")
{
    std::ostringstream oss;
    std::streambuf* p_cout_streambuf = std::cout.rdbuf();
    std::cout.rdbuf(oss.rdbuf());
    Input_Parser Parser;

    std::string input = "V 4";
    Parser.cmd_parser(input);
    input = "E {<0,1>}";
    Parser.cmd_parser(input);
    input = "s 0 1\n";
    Parser.cmd_parser(input);
    std::cout.rdbuf(p_cout_streambuf);

    CHECK(oss.str() == "0-1\n");
}

TEST_CASE("Test 3")
{
    std::ostringstream oss;
    std::streambuf* p_cout_streambuf = std::cout.rdbuf();
    std::streambuf* p_cerr_streambuf = std::cerr.rdbuf();
    std::cout.rdbuf(oss.rdbuf());
    std::cerr.rdbuf(oss.rdbuf());
    Input_Parser Parser;

    std::string input = "V 4";
    Parser.cmd_parser(input);
    input = "E {<0,1>}";
    Parser.cmd_parser(input);
    input = "s 0 0";
    Parser.cmd_parser(input);


    CHECK(oss.str() == "0-0\n");

    oss.str("");
    input = "s 1 1";
    Parser.cmd_parser(input);
    CHECK(oss.str() == "1-1\n");

    oss.str("");
    input = "s 3 3";
    Parser.cmd_parser(input);
    CHECK(oss.str() == "3-3\n");

    oss.str("");
    input = "s 0 0";
    Parser.cmd_parser(input);
    CHECK(oss.str() == "0-0\n");


    oss.str("");
    input = "s 4 4";
    Parser.cmd_parser(input);
    CHECK(oss.str().find("Error:") != std::string::npos);

    oss.str("");
    input = "s 2 4";
    Parser.cmd_parser(input);
    CHECK(oss.str().find("Error:") != std::string::npos);


    std::cout.rdbuf(p_cout_streambuf);
    std::cerr.rdbuf(p_cerr_streambuf);
}

TEST_CASE("Test 4")
{
    std::ostringstream oss;
    std::streambuf* p_cout_streambuf = std::cout.rdbuf();
    std::streambuf* p_cerr_streambuf = std::cerr.rdbuf();
    std::cout.rdbuf(oss.rdbuf());
    std::cerr.rdbuf(oss.rdbuf());

    Input_Parser Parser;

    std::string input = "V 4";
    Parser.cmd_parser(input);
    input = "E {<0,1>}";
    Parser.cmd_parser(input);
    input = "E {<0,2>}";
    Parser.cmd_parser(input);

    CHECK(oss.str().find("Error:") != std::string::npos);

    std::cout.rdbuf(p_cout_streambuf);
    std::cerr.rdbuf(p_cerr_streambuf);
}

TEST_CASE("Test 5")
{
    std::ostringstream oss;
    std::streambuf* p_cout_streambuf = std::cout.rdbuf();
    std::streambuf* p_cerr_streambuf = std::cerr.rdbuf();
    std::cout.rdbuf(oss.rdbuf());
    std::cerr.rdbuf(oss.rdbuf());

    Input_Parser Parser;

    std::string input = "V 4";
    Parser.cmd_parser(input);
    input = "E {<0,1>,<0,1>,<0,2>}";
    Parser.cmd_parser(input);
    input = "s 1 0";
    Parser.cmd_parser(input);

    CHECK(oss.str() == "1-0\n");

    std::cout.rdbuf(p_cout_streambuf);
    std::cerr.rdbuf(p_cerr_streambuf);
}

TEST_CASE("Test 6")
{
    std::ostringstream oss;
    std::streambuf* p_cout_streambuf = std::cout.rdbuf();
    std::streambuf* p_cerr_streambuf = std::cerr.rdbuf();
    std::cout.rdbuf(oss.rdbuf());
    std::cerr.rdbuf(oss.rdbuf());

    Input_Parser Parser;

    std::string input = "V 1";
    Parser.cmd_parser(input);


    CHECK(oss.str().find("Error:") != std::string::npos);

    std::cout.rdbuf(p_cout_streambuf);
    std::cerr.rdbuf(p_cerr_streambuf);
}

TEST_CASE("Test 7")
{
    std::ostringstream oss;
    std::streambuf* p_cout_streambuf = std::cout.rdbuf();
    std::streambuf* p_cerr_streambuf = std::cerr.rdbuf();
    std::cout.rdbuf(oss.rdbuf());
    std::cerr.rdbuf(oss.rdbuf());

    Input_Parser Parser;

    std::string input = "V 5";
    Parser.cmd_parser(input);
    input = "E {<0,2>,<2,1>,<2,3>,<3,4>,<4,1>}";
    Parser.cmd_parser(input);
    input = "s 0 0";
    Parser.cmd_parser(input);


    CHECK(oss.str() == "0-0\n");

    std::cout.rdbuf(p_cout_streambuf);
    std::cerr.rdbuf(p_cerr_streambuf);
}

TEST_CASE("Test 8")
{
   std::ostringstream oss;
   std::streambuf* p_cout_streambuf = std::cout.rdbuf();
   std::streambuf* p_cerr_streambuf = std::cerr.rdbuf();
   std::cout.rdbuf(oss.rdbuf());
   std::cerr.rdbuf(oss.rdbuf());

   Input_Parser Parser;

   std::string input = "V 5";
   Parser.cmd_parser(input);
   input = "E {<1,6>}";
   Parser.cmd_parser(input);
   CHECK(oss.str().find("Error:") != std::string::npos);

   oss.str("");
   input = "V 5";
   Parser.cmd_parser(input);
   input = "E {<0,2>,<2,1>,<2,3>,<3,4>,<4,1>}";
   Parser.cmd_parser(input);
   input = "s 0 0";
   Parser.cmd_parser(input);

   CHECK(oss.str() == "0-0\n");

    std::cout.rdbuf(p_cout_streambuf);
    std::cerr.rdbuf(p_cerr_streambuf);
}

TEST_CASE("Test 9")
{
   std::ostringstream oss;
   std::streambuf* p_cout_streambuf = std::cout.rdbuf();
   std::streambuf* p_cerr_streambuf = std::cerr.rdbuf();
   std::cout.rdbuf(oss.rdbuf());
   std::cerr.rdbuf(oss.rdbuf());

   Input_Parser Parser;

   std::string input = "V 5";
   Parser.cmd_parser(input);
   input = "E {<0,2>,<2,1>,<2,3>,<3,4>,<4,1>}";
   Parser.cmd_parser(input);
   input = "s 5 2";
   Parser.cmd_parser(input);
   CHECK(oss.str().find("Error:") != std::string::npos);


   std::cout.rdbuf(p_cout_streambuf);
   std::cerr.rdbuf(p_cerr_streambuf);
}


//This test case fails. Consider fixing?
TEST_CASE("Test 10")
{
   std::ostringstream oss;
   std::streambuf* p_cout_streambuf = std::cout.rdbuf();
   std::streambuf* p_cerr_streambuf = std::cerr.rdbuf();
   std::cout.rdbuf(oss.rdbuf());
   std::cerr.rdbuf(oss.rdbuf());

   Input_Parser Parser;

   std::string input = "V 5";
   Parser.cmd_parser(input);
   input = "E {<0,2>,<2,1>,<2,3>,<3,4>,<0,0>}";
   Parser.cmd_parser(input);
   input = "s 1 4";
   Parser.cmd_parser(input);
   CHECK(oss.str() == "1-2-3-4\n");


   std::cout.rdbuf(p_cout_streambuf);
   std::cerr.rdbuf(p_cerr_streambuf);
}

TEST_CASE("Test 11")
{
   std::ostringstream oss;
   std::streambuf* p_cout_streambuf = std::cout.rdbuf();
   std::streambuf* p_cerr_streambuf = std::cerr.rdbuf();
   std::cout.rdbuf(oss.rdbuf());
   std::cerr.rdbuf(oss.rdbuf());

   Input_Parser Parser;

   std::string input = "V 15";
   Parser.cmd_parser(input);
   input = "E {<2,6>,<2,8>,<2,5>,<6,5>,<5,8>,<6,10>,<10,8>}";
   Parser.cmd_parser(input);
   input = "s 2 10";
   Parser.cmd_parser(input);
   CHECK(oss.str() == "2-6-10\n");

   oss.str("");
   input = "V 5";
   Parser.cmd_parser(input);
   input = "E {<0,2>,<2,1>,<2,3>,<3,4>,<4,1>}";
   Parser.cmd_parser(input);
   input = "s 4 0";
   Parser.cmd_parser(input);
   CHECK(oss.str() == "4-3-2-0\n");



   std::cout.rdbuf(p_cout_streambuf);
   std::cerr.rdbuf(p_cerr_streambuf);
}

TEST_CASE("Test 12")
{
   std::ostringstream oss;
   std::streambuf* p_cout_streambuf = std::cout.rdbuf();
   std::streambuf* p_cerr_streambuf = std::cerr.rdbuf();
   std::cout.rdbuf(oss.rdbuf());
   std::cerr.rdbuf(oss.rdbuf());

   Input_Parser Parser;

   std::string input = "V 5";
   Parser.cmd_parser(input);
   input = "E {<1,2>,<2,4>,<4,3>}";
   Parser.cmd_parser(input);
   input = "s 1 4";
   Parser.cmd_parser(input);
   CHECK(oss.str() == "1-2-4\n");

   oss.str("");
   input = "s 3 4";
   Parser.cmd_parser(input);
   CHECK(oss.str() == "3-4\n");



   std::cout.rdbuf(p_cout_streambuf);
   std::cerr.rdbuf(p_cerr_streambuf);
}

TEST_CASE("Test 13")
{
   std::ostringstream oss;
   std::streambuf* p_cout_streambuf = std::cout.rdbuf();
   std::streambuf* p_cerr_streambuf = std::cerr.rdbuf();
   std::cout.rdbuf(oss.rdbuf());
   std::cerr.rdbuf(oss.rdbuf());

   Input_Parser Parser;

   std::string input = "V 15";
   Parser.cmd_parser(input);
   input = "E {<2,6>,<2,8>,<2,5>,<6,5>,<5,8>,<6,10>,<10,8>}";
   Parser.cmd_parser(input);
   input = "s 2 10";
   Parser.cmd_parser(input);
   CHECK(oss.str() == "2-6-10\n");

   oss.str("");
   input = "V 5";
   Parser.cmd_parser(input);
   input = "E {<0,2>,<2,1>,<2,3>,<3,4>,<4,1>}";
   Parser.cmd_parser(input);
   input = "s 4 0";
   Parser.cmd_parser(input);
   CHECK(oss.str() == "4-3-2-0\n");

   oss.str("");
   input = "s 4 5";
   Parser.cmd_parser(input);
   CHECK(oss.str().find("Error:") != std::string::npos);




   std::cout.rdbuf(p_cout_streambuf);
   std::cerr.rdbuf(p_cerr_streambuf);
}

TEST_CASE("Test 14")
{
   std::ostringstream oss;
   std::streambuf* p_cout_streambuf = std::cout.rdbuf();
   std::streambuf* p_cerr_streambuf = std::cerr.rdbuf();
   std::cout.rdbuf(oss.rdbuf());
   std::cerr.rdbuf(oss.rdbuf());

   Input_Parser Parser;

   std::string input = "V 15";
   Parser.cmd_parser(input);
   input = "E {<2,6>,<2,8>,<2,5>,<6,5>,<5,8>,<6,10>,<10,8>}";
   Parser.cmd_parser(input);
   input = "s 2 12";
   Parser.cmd_parser(input);


   CHECK(oss.str().find("Error:") != std::string::npos);


   std::cout.rdbuf(p_cout_streambuf);
   std::cerr.rdbuf(p_cerr_streambuf);
}




    // std::cout.rdbuf(oss.rdbuf());
    //
    // Parser.cmd_parser(input);
    // std::cout.rdbuf(p_cout_streambuf);
    // CHECK(oss.str() == "0-1\n");
    //
    // std::cout.rdbuf(oss.rdbuf());
    // input = "s 0 0";
    // Parser.cmd_parser(input);
    // std::cout.rdbuf(p_cout_streambuf);
    // CHECK(oss.str() == "0-0\n");
    //
    // std::cout.rdbuf(oss.rdbuf());
    // input = "s 1 1";
    // Parser.cmd_parser(input);
    // std::cout.rdbuf(p_cout_streambuf);
    // CHECK(oss.str() == "1-1\n");
    //
    // std::cout.rdbuf(oss.rdbuf());
    // input = "s 4 4";
    // Parser.cmd_parser(input);
    // std::cout.rdbuf(p_cout_streambuf);
    // CHECK(oss.str().find("Error:") != std::string::npos);





TEST_CASE("Successful Test Example") {
    int a = 5;
    CHECK(a == 5);
}


// TEST_CASE("Failing Test Examples") {
//     CHECK(true == false);
// }
