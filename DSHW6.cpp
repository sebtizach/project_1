#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include <locale>

//I declare some constants here, which will be of use later in my switch statement
enum constraint_direction {across_dir, down_dir, either_dir};

//Function declarations
int find_solutions(const std::vector<std::string>& dict, std::vector<std::vector<char>> grid,
  std::vector<int> across, std::vector<int> down, std::vector<int> either,
  const std::vector<std::vector<char>>& original_grid, int first_i, int first_j, std::string solution_mode, int output_decider);

void output_solutions(std::vector<std::vector<char> > grid, std::vector<std::vector<char> > original_grid);
int case_count = 0;
int final_count = 0;
std::vector<std::string> taken_words;
int find_solutions(const std::vector<std::string>& dict, std::vector<std::vector<char>> grid,
  std::vector<int> across, std::vector<int> down, std::vector<int> either,
  const std::vector<std::vector<char>>& original_grid, int first_i, int first_j, std::string solution_mode, int output_decider)
{
  int solution_count = 0;
  //If these vectors are empty, you have reached the base case
  if (across.empty() && down.empty() && either.empty())
  {
    if(solution_mode == "one_solution" && case_count == 1){
      return 1;
    }
    else{
      int counter;
      if(output_decider == 0)
      {
        bool valid = true;
        for(unsigned int i = 0; i < taken_words.size(); i++){
          //The code in this loop checks for repeat words
          counter = 0;
          counter = std::count(taken_words.begin(), taken_words.end(), taken_words[i]);
          if(counter > 1){
            valid = false;
          }
        }
        if(valid == true){
          output_solutions(grid, original_grid);
          //Increment these counters based off of whether or not the solution was valid
          final_count += 1;
          case_count += 1;
        }
      }
      else{
        //Do the same process, but don't output.  A bit repetative, but I believe that it helps visualize the process better
        bool valid = true;
        int counter;
        for(unsigned int i = 0; i < taken_words.size(); i++){
          counter = 0;
          counter = std::count(taken_words.begin(), taken_words.end(), taken_words[i]);
          if(counter > 1){
            valid = false;
          }
        }
        if(valid == true){
          final_count += 1;
          case_count += 1;
        }
      }
      return 1;
    }
  }

  //Declare vectors with the appropriate constraints
  std::vector<int> unique_across = across;
  std::sort( unique_across.begin(), unique_across.end());
  unique_across.erase( std::unique( unique_across.begin(), unique_across.end()), unique_across.end());

  std::vector<int> unique_down = down;
  std::sort( unique_down.begin(), unique_down.end());
  unique_down.erase( std::unique(unique_down.begin(), unique_down.end()), unique_down.end());

  std::vector<int> unique_either = either;
  std::sort( unique_either.begin(), unique_either.end());
  unique_either.erase( std::unique(unique_either.begin(), unique_either.end()), unique_either.end());

  unsigned int i = first_i, j = first_j;
  if(j >= grid[i].size())
  {
    j = 0;
    i++;
  }

  while(i < grid.size())
  {
    while(j < grid[i].size())
    {
      // loop over all unique lengths in across and down
      // if current letter != 1 && != 3 && word of length (length) across is in dict
      // recursive call, if current position == 2 set to 3 else 1
      // if current letter != 2 && != 3 && word of length (length) down is in dict
      // recursive call, if current position == 1 set to 3 else 2

      std::vector<std::vector<char>> new_grid = grid;
      std::vector<int> new_across = across;
      std::vector<int> new_down = down;

      for(unsigned int k=0; k<unique_across.size(); k++)
      {
        if(grid[i][j] != '1' && grid[i][j] != '3' && unique_across[k]+j-1 < grid[i].size()) 
        {
          std::string curr_word;
          for(int l=0; l<unique_across[k]; l++)
          {
            curr_word += original_grid[i][j+l];
          }
          //This if statement runs when a word is found
          if(std::find(dict.begin(), dict.end(), curr_word) != dict.end())
          {
            std::vector<std::vector<char>> new_grid = grid;
            for(int l=0; l<unique_across[k]; l++)
            {
              if(new_grid[i][j+l] == '2')
              {
                new_grid[i][j+l] = '3';
              }
              else
              {
                new_grid[i][j+l] = '1';
              }
            }
            for(unsigned int l_remove=0; l_remove < across.size(); l_remove++)
            {
              if(new_across[l_remove] == unique_across[k])
              {
                new_across.erase(new_across.begin() + l_remove);
                break;
              }
            }
            solution_count += find_solutions(dict, new_grid, new_across, new_down, either, original_grid, i, j+1, solution_mode, output_decider);
          }
        }
      }
      for(unsigned int k=0; k<unique_down.size(); k++)
      {
        if(grid[i][j] != '2' && grid[i][j] != '3' && unique_down[k]+i-1 < grid.size())
        {
          std::string curr_word;
          for(int l=0; l<unique_down[k]; l++)
          {
            curr_word += original_grid[i+l][j];
          }
          if(std::find(dict.begin(), dict.end(), curr_word) != dict.end())
          {
            taken_words.push_back(curr_word);
            std::vector<std::vector<char>> new_grid = grid;
            for(int l=0; l<unique_down[k]; l++)
            {
              if(new_grid[i+l][j] == '1')
              {
                new_grid[i+l][j] = '3';
              }
              else
              {
                new_grid[i+l][j] = '2';
              }
            }
            std::vector<int> new_down = down;
            for(unsigned int l_remove=0; l_remove < down.size(); l_remove++)
            {
              if(new_down[l_remove] == unique_down[k])
              {
                new_down.erase(new_down.begin() + l_remove);
                break;
              }
            }
            solution_count += find_solutions(dict, new_grid, new_across, new_down, either, original_grid, i, j+1, solution_mode, output_decider);
            taken_words.clear();
          }
        }
      }

      std::vector<std::vector<char>> across_grid = grid;
      std::vector<std::vector<char>> down_grid = grid;
      
      for(unsigned int k=0; k<unique_either.size(); k++)
      {
        //Here I initialize two current words that run in either direction at the same time
        std::string curr_word_across;
        std::string curr_word_down;
        if(grid[i][j] != '1' && grid[i][j] != '3' && unique_either[k]+j-1 < grid[i].size())
        {
          for(int l=0; l<unique_either[k]; l++)
          {
            curr_word_across += original_grid[i][j+l];
          }
        }
        if(grid[i][j] != '2' && grid[i][j] != '3' && unique_either[k]+i-1 < grid.size())
        {
          for(int l=0; l<unique_either[k]; l++)
          {
            curr_word_down += original_grid[i+l][j];
          }
        }

        bool found_across = std::find(dict.begin(), dict.end(), curr_word_across) != dict.end();
        bool found_down = std::find(dict.begin(), dict.end(), curr_word_down) != dict.end();

        if(found_across && found_down)
        {
          std::vector<int> new_either = either;
          for(unsigned int l_remove=0; l_remove < either.size(); l_remove++)
          {
            if(new_either[l_remove] == unique_either[k])
            {
              new_either.erase(new_either.begin() + l_remove);
              break;
            }
          }
          for(int l=0; l<unique_either[k]; l++)
          {
            if(across_grid[i][j+l] == '2')
            {
              across_grid[i][j+l] = '3';
            }
            else
            {
              across_grid[i][j+l] = '1';
            }
          }
          for(int l=0; l<unique_either[k]; l++)
          {
            if(down_grid[i+l][j] == '1')
            {
              down_grid[i+l][j] = '3';
            }
            else
            {
              down_grid[i+l][j] = '2';
            }
          }
          
          //Increment twice because this is the either case
          solution_count += find_solutions(dict, across_grid, new_across, new_down, new_either, original_grid, i, j+1, solution_mode, output_decider);
          solution_count += find_solutions(dict, down_grid, new_across, new_down, new_either, original_grid, i, j+1, solution_mode, output_decider);

          if(std::find(new_either.begin(), new_either.end(), unique_either[k]) != new_either.end())
          {
            for(unsigned int l_remove=0; l_remove < either.size(); l_remove++)
            {
              if(new_either[l_remove] == unique_either[k])
              {
                new_either.erase(new_either.begin() + l_remove);
                break;
              }
            }
            for(int l=0; l<unique_either[k]; l++)
            {
              if(new_grid[i][j+l] == '2')
              {
                new_grid[i][j+l] = '3';
              }
              else
              {
                new_grid[i][j+l] = '1';
              }
            }
            for(int l=0; l<unique_either[k]; l++)
            {
              if(new_grid[i+l][j] == '1')
              {
                new_grid[i+l][j] = '3';
              }
              else
              {
                new_grid[i+l][j] = '2';
              }
            }
            solution_count += find_solutions(dict, new_grid, new_across, new_down, new_either, original_grid, i, j+1, solution_mode, output_decider);
          }
        }
        else if(found_across)
        {
          std::vector<int> new_either = either;
          for(unsigned int l_remove=0; l_remove < either.size(); l_remove++)
          {
            if(new_either[l_remove] == unique_either[k])
            {
              new_either.erase(new_either.begin() + l_remove);
              break;
            }
          }
          for(int l=0; l<unique_either[k]; l++)
          {
            if(across_grid[i][j+l] == '2')
            {
              across_grid[i][j+l] = '3';
            }
            else
            {
              across_grid[i][j+l] = '1';
            }
          }
          solution_count += find_solutions(dict, across_grid, new_across, new_down, new_either, original_grid, i, j+1, solution_mode, output_decider);
        }
        else if(found_down)
        {
          std::vector<int> new_either = either;
          for(unsigned int l_remove=0; l_remove < either.size(); l_remove++)
          {
            if(new_either[l_remove] == unique_either[k])
            {
              new_either.erase(new_either.begin() + l_remove);
              break;
            }
          }
          for(int l=0; l<unique_either[k]; l++)
          {
            if(down_grid[i+l][j] == '1')
            {
              down_grid[i+l][j] = '3';
            }
            else
            {
              down_grid[i+l][j] = '2';
            }
          }
          solution_count += find_solutions(dict, down_grid, new_down, new_down, new_either, original_grid, i, j+1, solution_mode, output_decider);
        }
        else{
          continue;
        }
      }
      j++;
    }
    j = 0;
    i++;
  }
  return solution_count;
}



void output_solutions(std::vector<std::vector<char>> grid, std::vector<std::vector<char>> original_grid)
{
  //Once the base case is reached, the resulting grid and the original puzzle grid are thrown in here.
  //The resulting grid has every letter involved with a matching word marked as a number, per my method above.
  //Then, all you need to do is compare the two grids, and if a character on the new grid is the same as a
  //character on the old grid, that means it was not changed to a number and thus is not part of a matching
  //word, in which case you replace it with a #.
  std::cout << "Board:" << std::endl;
  for(unsigned int i=0; i<grid.size(); i++)
  {
    for(unsigned int j=0; j<grid[i].size(); j++)
    {
      if(grid[i][j] == original_grid[i][j])
      {
        std::cout << '#';
      }
      else
      {
        std::cout << original_grid[i][j];
      }
    }
    std::cout << std::endl;
  }
}

int main(int argc, char* argv[]) {

  //Simple initialization of the inputs
  std::ifstream dict_input(argv[1]);
  std::ifstream input(argv[2]);
  std::string solution_mode = argv[3];
  std::string output = argv[4];
  std::vector<std::string> dict;
  //Take the info from the input file
  while(dict_input.is_open() && dict_input.good()) {
    std::string new_word;
    std::getline(dict_input, new_word);
    if(new_word != "") {
      dict.push_back(new_word);
    }
  }
  std::vector<std::vector<char>> grid;
  //Declare vectors
  std::vector<int> across;
  std::vector<int> down;
  std::vector<int> either;

  constraint_direction curr_direction = either_dir;
  while(input.is_open() && input.good()) {
    std::string str;
    std::getline(input, str);
    if(str[0] == '!') {
      std::string make_lower = str.substr(1);
      for(unsigned int i=0; i<make_lower.length(); i++) {
        make_lower[i] = std::tolower(make_lower[i]);
      }
      if(make_lower == "across") {
        curr_direction = across_dir;
      } else if(make_lower == "down") {
        curr_direction = down_dir;
      }
    }
    else if(str[0] == '+') {
      //This switch statement checks to see if any of these cases are met within the
      //provided info.  The integer following the '+' is isolated and thrown into the
      //matching vector as a constraint.
      switch(curr_direction) {
        case across_dir:
          across.push_back(std::stoi(str.substr(1)));
          break;
        case down_dir:
          down.push_back(std::stoi(str.substr(1)));
          break;
        default:
          either.push_back(std::stoi(str.substr(1)));
      }
    }
    else if(str != ""){
      std::vector<char> grid_line(str.begin(), str.end());
      grid.push_back(grid_line);
    }
  }
  if(across.size() > 0 && down.size() == 0){
    down = across;
  }
  else if(down.size() > 0 && across.size() == 0){
    across = down;
  }
  if(output == "print_boards" || output == "count_only"){
    if(solution_mode == "all_solutions")
    {
      find_solutions(dict, grid, across, down, either, grid, 0, 0, solution_mode, 1);
      std::cout << "Number of Solutions: " << final_count << std::endl;
      if(output == "print_boards")
      {
        find_solutions(dict, grid, across, down, either, grid, 0, 0, solution_mode, 0);
      }
    }
    else if(solution_mode == "one_solution")
    {
      find_solutions(dict, grid, across, down, either, grid, 0, 0, solution_mode, 1);
      if (final_count > 1)
      {
        final_count = 1;
      }
      std::cout << "Number of Solutions: " << final_count << std::endl;
      if(output == "print_boards")
      {
        case_count = 0;
        find_solutions(dict, grid, across, down, either, grid, 0, 0, solution_mode, 0);
      }
    }
    //Here we have some basic error messages
    else
    {
      std::cerr << "ERROR:  Invalid solution mode, must be 'all_solutions' or 'one_solution' " << std::endl;
    }
  }
  else
  {
    std::cerr << "ERROR:  Invalid output mode, must be 'print_boards' or 'count_only' " << std::endl;
  }
  //Close the input files once the code has finished
  dict_input.close();
  input.close();
  return 0;
}