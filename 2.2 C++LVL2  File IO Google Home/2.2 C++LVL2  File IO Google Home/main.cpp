#include <iostream>   // Our list of directives telling the preprocessor to locate and copy the contents of these files firectily into this code here. 
#include <stdexcept>  // This is done selectively to consider ram, cpu, compile time, file size, efficient use of resources, scalability and maintainability. 
#include <string>
#include <vector>
#include <sstream>
#include <fstream> // this fstream directive allows us access to use file stream classes such as ifstream and ofstream to input and output file streams. 

using namespace std;  // Groups all standard library functions, classes, and objects to aboid naming conflicts with other libraries or user-defined codes. Conflicts in larger projects where naming conflicts may arise from my research however. 

// Display program instructions
void displayInstructions()\
{
    cout << "Personal Expense Tracker to keep you organized and well-balanced!" << endl;
    cout << "Log your expenses by categorizing them and adding the amount. Type 'done' when ready for a summary.\n" << endl;
}

struct Expense // this data structure will be instantiated as expense throught the program then stored in the expenses vector becuase it's not part of the class... 
{
    string category;
    double amount = 0.0;
};
// ExpenseTracker is using 2 main methods for file I/O: loadExpensesFromFile and saveExpensesToFile
class ExpenseTracker 
{
private:
    vector<Expense> expenses;
    // this is a non-return function which takes a constant reference to string named filename.  This method is within the ExpenseTracker class
    void loadExpensesFromFile(const string& filename) 
    {
        /* ifstream(input file stream) class is used to create an object named file, with filename passed as the argument.
         This object creates an input file stream */
        ifstream file(filename); 
        if (file.is_open()) // if condition to verify if the file object successfully opened and ready for reading.
        {
            Expense expense; // 1st instantiation to hold data for category and amount vefore adding them to the expense vector...
            while (file >> expense.category >> expense.amount) // while loop attempts to read category and amout values from the file into the expense object on each iteration.
            {
                expenses.push_back(expense); // expense values are being referenced to the expenses vector..
            }
            /* output to console letting the user know that there is a record of expenses in a stored file. Using \n to begin move down without needing to flush buffer
               becuase it is faster in this case to get the program flow going as the next else block output will resemble the same concept for \n */
            cout << "There are expenses loaded from file.  Continue to add below:\n"; 
        }
        else  // else statement executes if no file exists and results in this console output.
        {
            cout << "No existing expense file found. Starting fresh.\n";
        }
    }
    // this is a void non-return constant function that passes a constant string reference named filename. Purpose is to write expense data to the specified file. 
    void saveExpensesToFile(const string& filename) const 
                                                          
    {
        ofstream file(filename); // ofstream named file with filename passed as the parameter. output file stream creates or opens expenses.txt preparing it for writing.
        for (const auto& expense : expenses) // for loop iterating each expense into the expenses vector.
        {
            file << expense.category << " " << expense.amount << "\n"; // output to file that writes expense category and amount, each on a new line. Stored in exense.txt

        }
        cout << "Expenses saved to file.\n"; // output to console letting the user that expenses have been saved..
    }

public: // public access specifier for ExpenseTracker class.  Allowing access to its constructors members.  
    ExpenseTracker(const string& filename) // constant that initializes the ExpenseTracker class to create string referenced objects named filename
    {        
        loadExpensesFromFile(filename); // Calls the loadExpensesFromFile function with filename argument to load existing expenses.
    }

    void addExpense(const string& category, const string& inputAmount)
    {
        double amount;
        stringstream ss(inputAmount);

        if (!(ss >> amount)) 
        {
            throw invalid_argument("Error: Invalid input. Please enter a numeric value for the amount.");
        }
        if (amount < 0)
        {
            throw out_of_range("Error: Expense amount cannot be negative.");
        }

        Expense expense{ category, amount }; // initializes an Expense instance assigning category and amount as parameters... 
        expenses.push_back(expense);
        cout << "Added expense: " << expense.category << " - $" << expense.amount << endl;
    }

    //  constant searchExpense function that takes searchCategory as a constant string reference.  Option function fulfilled..
    void searchExpense(const string& searchCategory) const 
    {
        bool found = false; // boolean flag named found initialized to false and tracks if any matching category is found during the for loop iteration 
        for (const Expense& expense : expenses) // for loop iterating each Expense object in the expenses vector as a constant referenced as expense.
        {
            if (expense.category == searchCategory) // if condition checking if expense.category matches what the user is searching for which is typed in searchCategory
            {
                cout << "- " << expense.category << ": $" << expense.amount << endl;  // console out printing expense values that match the users search input
                found = true; // this flag confirms that a match has been found and setting found to true. Taking it to the next if condition and bypassing it
            }
        }
        if (!found) // if statement evaluating with a NOT operator to negate the value of found. Thus, if, not-found output: cout...
        {
            cout << "No expenses found in category: " << searchCategory << endl; // prints to the console this string literal and users search input
        }
    }

    void calculateTotal() const
    {
        if (expenses.empty())
        {
            throw runtime_error("Error: No expenses recorded. Please add expenses before calculating the total.");
        }

        double total = 0;
        for (const auto& expense : expenses) 
        {
            total += expense.amount;
        }
        cout << "\nTotal spending: $" << total << endl;
    }

    void displayExpenses() const 
    {
        if (expenses.empty())
        {
            cout << "No expenses recorded yet." << endl;
            return;
        }
        cout << "\nRecorded Expenses:" << endl;
        for (const auto& expense : expenses)
        {
            cout << "- " << expense.category << ": $" << expense.amount << endl;
        }
    }
    // constant non-return function saveToFile that takes a constant string reference parameter named filename.
    void saveToFile(const string& filename) const 
    {
        saveExpensesToFile(filename); /* calls the saveExpensesToFile function passing filename as the argument. 
                                  saveToFile function is used here to give better UX designed with maintainability and readability in mind. */
    }
};

int main()
{
    displayInstructions();

    string filename = "expenses.txt"; // string variable named filename initialized with "expenses.txt" as the file title for storing the expenses data
    ExpenseTracker tracker(filename); //  ExpenseTracker object named tracker is instantiated with filename being passed to its constructor 
    bool success = true;

    try
    {
        string category, amount;
        while (true) 
        {
            cout << "\nEnter an expense category (or type 'SEARCH' to look up a category. Otherwise 'DONE' to finish): "; // SEARCH is now included in the string 
            getline(cin, category);
            if (category == "DONE") break;
            if (category == "SEARCH") // if statement checking if category is equal to SEARCH through user input, if so, then the following code is executed
            {
                cout << "Enter category to search: "; // output to the user instructing them
                string searchCategory; // string variable named searchCategory
                getline(cin, searchCategory); // getline function captures user input and stores it in searchCategory. Using getline allows multiple words. 
                tracker.searchExpense(searchCategory); // tracker calls searchExpense function passing searchCategory argument
                continue; // continue statement so the program skips over any remaining code in the loop and proceeds to the next iteration of the while loop. 
            }

            cout << "Enter the amount spent on " << category << ": $";
            cin >> amount;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            tracker.addExpense(category, amount);
        }

        tracker.displayExpenses();
        tracker.calculateTotal();
    }
    catch (const invalid_argument& e) 
    {
        cout << e.what() << endl;
        success = false;
    }
    catch (const out_of_range& e)
    {
        cout << e.what() << endl;
        success = false;
    }
    catch (const runtime_error& e) 
    {
        cout << e.what() << endl;
        success = false;
    }

    if (success) 
    {
        tracker.saveToFile(filename);  // if successful then tracker calls saveToFile function with filename passed as an argument to specify where to save data..
        cout << "\nGreat job on staying on top of your finances!" << endl;
    }

    return 0;
}
