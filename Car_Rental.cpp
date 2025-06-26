#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <cstdio>


using namespace std;
string Cars_Database_Path="CARS.csv";
string Customers_Database_Path="CUSTOMERS.csv";
string Employees_Database_Path="EMPLOYEES.csv";

int stointeger(string s){
    stringstream ss(s);
    int x = 0;
    
    if (!(ss >> x)) {
        // Conversion failed, set result to 0 and provide a disclaimer
        cout << "Warning: Invalid string encountered. Conversion result set to 0." << endl;
        x = 0;
    }
    
    return x;
}

int dayDifference(const std::string& date1, const std::string& date2) {
    std::tm timeinfo1 = { 0 };
    std::tm timeinfo2 = { 0 };

    // Parse the first date
    sscanf(date1.c_str(), "%d/%d/%d", &timeinfo1.tm_mday, &timeinfo1.tm_mon, &timeinfo1.tm_year);
    timeinfo1.tm_mon -= 1;
    timeinfo1.tm_year -= 1900;

    // Parse the second date
    sscanf(date2.c_str(), "%d/%d/%d", &timeinfo2.tm_mday, &timeinfo2.tm_mon, &timeinfo2.tm_year);
    timeinfo2.tm_mon -= 1;
    timeinfo2.tm_year -= 1900;

    // Convert to time_t
    std::time_t time1 = std::mktime(&timeinfo1);
    std::time_t time2 = std::mktime(&timeinfo2);

    // Calculate the difference in seconds
    std::time_t difference = std::difftime(time2, time1);

    // Convert seconds to days
    int daysDifference = difference / (60 * 60 * 24);

    return daysDifference;
}

bool isValidDate(int day, int month, int year) {
    if (year < 1900) {
        return false; // Assuming years before 1900 are not valid
    }

    if (month < 1 || month > 12) {
        return false;
    }

    // Check the number of days in the given month
    int daysInMonth;
    switch (month) {
        case 4: case 6: case 9: case 11:
            daysInMonth = 30;
            break;
        case 2:
            daysInMonth = (year % 4 == 0 && (year % 100 != 0 || year % 400 == 0)) ? 29 : 28;
            break;
        default:
            daysInMonth = 31;
            break;
    }

    return (day >= 1 && day <= daysInMonth);
}

std::string convertToDate() {
    int day, month, year;
    while(1){
        

        std::cout << "Enter day: ";
        std::cin >> day;

        std::cout << "Enter month: ";
        std::cin >> month;

        std::cout << "Enter year: ";
        std::cin >> year;

        if (isValidDate(day, month, year)) {
            break;
        } else {
            std::cout << "Invalid date entered. Please Re-Enter." << std::endl;
            continue;
        }
    }
    
    std::tm timeinfo = { 0 };
    timeinfo.tm_mday = day;
    timeinfo.tm_mon = month - 1;
    timeinfo.tm_year = year - 1900;

    std::mktime(&timeinfo);

    char buffer[11];
    std::strftime(buffer, sizeof(buffer), "%d/%m/%Y", &timeinfo);

    return std::string(buffer);
}



class User{
    protected:
        string Name;
        string ID;
        string password;

    public:
        
        User(string name, string id, string pass) : Name(name), ID(id), password(pass) {};
        
        string database_path;

        virtual bool Read_CARS(){
            
            std::ifstream CARS;
            CARS.open(Cars_Database_Path); // Assuming Cars_Database_Path is a global variable

            if (!CARS.is_open()) {
                std::cerr << "Error: Unable to open file CARS.csv" << std::endl;
                return false;
            } else {
                std::cout << "File CARS.csv Opened Successfully" << std::endl;
                std::cout<<"Car_Reg | Car_Model| Available | Damage | Rented_To_ID | Rent | Rented_Date | Required_Score"<<endl;
            }
            int flag=0;
            while (CARS.peek() != EOF) {
                std::string line;
                std::getline(CARS, line, '\n');

                // Use stringstream to tokenize the CSV line
                std::istringstream iss(line);
                std::vector<std::string> fields;

                std::string field;
                while (std::getline(iss, field, ',')) {
                    fields.push_back(field);
                }

                // Check if the vector has at least 3 elements and the third element is "T"
                if (fields.size() == 8 && fields[2] == "T") {
                    flag++;
                    // Display only records with the 3rd entry as "T"
                    for (const std::string& entry : fields) {
                        std::cout << entry << ' ';
                    }
                    std::cout << std::endl;
                }
            }
            if(flag==0){
                std::cout<<"No cars available for rent"<<endl;
            }
            CARS.close();
            return true;    
        }
        
        virtual bool Rent_Car() {
            std::string Car_Reg_No;
            std::string transaction_amount;

            std::cout << "Enter the Car Registration Number to Rent: ";
            std::cin >> Car_Reg_No;
            while(1){
                std::cout << "Enter transaction amount: ";
                std::cin >> transaction_amount;
                if(stointeger(transaction_amount)<0){
                    cout<<"Invalid Entry. Try again."<<endl;
                    continue;
                }
                else{
                    break;
                }
            }
            

            // Open CARS.csv for reading
            std::ifstream carsFile("CARS.csv");
            if (!carsFile.is_open()) {
                std::cerr << "Error: Unable to open file CARS.csv" << std::endl;
                return false;
            }

            // Open CUSTOMERS.csv/EMPLOYEES.csv for reading
            std::ifstream customersFile(database_path);
            if (!customersFile.is_open()) {
                std::cerr << "Error: Unable to open csv file " << std::endl;
                carsFile.close();
                return false;
            }

            // Open temporary file for writing
            std::ofstream tempFile("CARS_temp.csv");
            if (!tempFile.is_open()) {
                std::cerr << "Error: Unable to create temporary file." << std::endl;
                carsFile.close();
                customersFile.close();
                return false;
            }

            std::string line;
            bool carFound = false;
            int count=0;
            while (std::getline(carsFile, line)) {
                count++;
                if(count<=2){
                    tempFile << line<<"\n";
                    continue;
                }

                std::istringstream iss(line);
                std::string currentCarRegNo, CarModel, CarAvailable, CarDamage, CarRentedtoID, CarRent, CarRentedDate, CarRequiredScore;

                // Assuming Car_Reg_No is the first field in CARS.csv
                std::getline(iss, currentCarRegNo, ',');
                std::getline(iss, CarModel, ',');
                std::getline(iss, CarAvailable, ',');
                std::getline(iss, CarDamage, ',');
                std::getline(iss, CarRentedtoID, ',');
                std::getline(iss, CarRent, ',');
                std::getline(iss, CarRentedDate, ',');
                std::getline(iss, CarRequiredScore, '\n');

                if (currentCarRegNo == Car_Reg_No) {
                    // Car_Reg_No exists in CARS.csv
                    carFound = true;

                    // Open EMPLOYEES.csv/CUSTOMERS.csv to check the 5th field
                    std::string customerID;
                    while (std::getline(customersFile, line)) {
                        std::istringstream issCustomer(line);
                        std::string currentCustomerID, customerName, customerPassword,  customerFine, customerRecord;
                        // Assuming ID is the first field in CUSTOMERS.csv/EMPLOYEES.csv
                        std::getline(issCustomer, currentCustomerID, ',');

                        
                        if (currentCustomerID == ID) {
                            std::getline(issCustomer, customerName, ',');
                            std::getline(issCustomer, customerPassword, ',');
                            std::getline(issCustomer, customerFine, ',');
                            std::getline(issCustomer, customerRecord, ',');

                            // Check the condition: 5th field in CUSTOMERS.csv >= 8th field in CARS.csv
                            if ((stointeger(customerRecord) <= stointeger(CarRequiredScore)) && ((ID.substr(0,1)=="C")?(stointeger(transaction_amount) >= stointeger(CarRent)):stointeger(transaction_amount) >= 0.85* stointeger(CarRent)) && (CarAvailable == "T")) {
                                // Update the 3rd, 5th, and 7th fields in CARS.csv
                                CarRentedtoID = ID;
                                CarAvailable = "F";
                                std::cout << "Enter the date of renting the car: ";
                                CarRentedDate=convertToDate();

                                // Print a message indicating a successful rental
                                std::cout << "Car rented successfully  "<< std::endl;
                            } else {
                                // Print a message indicating insufficient customer record or insufficient transaction amount or car not available
                                std::cout << "Unable to rent car (insufficient transaction amount/car unavailable/Defaulter Score too high) " << std::endl;
                            }
                            break;  // Break out of the loop after finding the matching customer
                        }
                    }
                }

                // Write the unmodified line to the temporary file
                
                
                if(!currentCarRegNo.empty()){
                        tempFile << currentCarRegNo << ","
                        << CarModel << ","
                        << CarAvailable << ","
                        << CarDamage << ","
                        << CarRentedtoID << ","
                        << CarRent << ","
                        << CarRentedDate << ","
                        << CarRequiredScore << "\n";
                }

                
            }

            carsFile.close();
            customersFile.close();
            tempFile.close();

            if (!carFound) {
                // Print a message if the Car_Reg_No is not found in CARS.csv
                std::cout << "Car with Registration Number " << Car_Reg_No << " not found in CARS.csv." << std::endl;
                std::remove("CARS_temp.csv");  // Remove the temporary file
                return false;
            }

            // Replace the original file with the temporary file
            std::remove("CARS.csv");
            std::rename("CARS_temp.csv", "CARS.csv");

            return true;
    }


     bool Read_Rented_CARS(){
            std::ifstream CARS;
            CARS.open(Cars_Database_Path); // Assuming Cars_Database_Path is a global variable

            if (!CARS.is_open()) {
                std::cerr << "Error: Unable to open file CARS.csv" << std::endl;
                return false;
            } 
            else {
                std::cout << "File CARS.csv Opened Successfully" << std::endl;
            }
            int flag=0;
            while (CARS.peek() != EOF) {
                std::string line;
                std::getline(CARS, line, '\n');

                // Use stringstream to tokenize the CSV line
                std::istringstream iss(line);
                std::vector<std::string> fields;

                std::string field;
                while (std::getline(iss, field, ',')) {
                    fields.push_back(field);
                }
               
                // Check if the vector has at least 3 elements and the third element is "F"
                if (fields.size() == 8 && fields[2] == "F" && fields[4] == this->ID) {
                    flag++;
                    // Display only records with the 3rd entry as "F" and 5th entry as Customer_ID
                    for (const std::string& entry : fields) {
                        std::cout << entry << ' ';
                    }
                    std::cout << std::endl;
                }
            }
            if(flag==0){
                std::cout<<"No cars rented by the customer"<<endl;
            }
            CARS.close();
            return true;
        }


        
        
};

class Manager: public User{
    private:
        bool IsCarRegistrationUnique(const std::string& newRegistration) {
            std::ifstream inFile(Cars_Database_Path);
            if (!inFile.is_open()) {
                std::cerr << "Error: Unable to open file " << Cars_Database_Path << std::endl;
                return false;
            }

            std::string line;
            while (std::getline(inFile, line)) {
                std::istringstream iss(line);
                std::string existingRegistration;

                if (std::getline(iss, existingRegistration, ',')) {
                    if (existingRegistration == newRegistration) {
                        // Car Registration already exists in the file
                        inFile.close();
                        return false;
                    }
                }
            }

            inFile.close();
            return true;
        }

        bool IsIDUnique(const std::string& newCustomerID, string Database_Path) {
            std::ifstream inFile(Database_Path);
            if (!inFile.is_open()) {
                std::cerr << "Error: Unable to open file " << Database_Path << std::endl;
                return false;
            }

            std::string line;
            while (std::getline(inFile, line)) {
                std::istringstream iss(line);
                std::string existingCustomerID;

                if (std::getline(iss, existingCustomerID, ',')) {
                    if (existingCustomerID == newCustomerID) {
                        // Customer already exists in the file
                        inFile.close();
                        return false;
                    }
                }
            }

            inFile.close();
            return true;
        }
    
        bool IsCustomerIDInCars(const std::string& customerID) {
            std::ifstream carsFile(Cars_Database_Path);
            if (!carsFile.is_open()) {
                std::cerr << "Error: Unable to open file CARS.csv" << std::endl;
                return false;
            }

            std::string line;
            while (std::getline(carsFile, line)) {
                std::istringstream iss(line);
                std::string carID;

                // Assuming Customer_ID is the 5th field in CARS.csv
                for (int i = 0; i < 5; ++i) {
                    std::getline(iss, carID, ',');
                }

                if (carID == customerID) {
                    // Customer_ID occurs in the 5th field of a record in CARS.csv
                    carsFile.close();
                    return true;
                }
            }

            carsFile.close();
            return false;
        }
    public:
        static int Manager_Count;
        Manager() : User("John", "M001", "1234") {
            Manager_Count++;
        }
        bool Rent_Car(){
            cout<<"Functionality not available for Manager"<<endl;
            return false;
        }
        bool Read_CARS(){
            ifstream CARS;

            CARS.open(Cars_Database_Path);
            if (!CARS.is_open())
            {
                cout << "Error: File Open" << endl;
                return 1;
            }
            else if (CARS.fail()){
                cerr << "Error: Unable to open file " << Cars_Database_Path << endl;
                return 1;
            }
            else
            {
                cout << "File CARS.csv Opened Successfully" << endl;
            }

            while (CARS.peek() != EOF)
            {
                string line;
                getline(CARS, line, ',');
                if (line=="\n"){
                    cout << endl;
                }
                else
                {
                cout << line <<' ';
                }
            }
            CARS.close();
            return 0;
        }
        
        bool Insert_Car_Record(){
            std::ofstream CARS;
            CARS.open("CARS.csv", std::ios::app);

            if (!CARS.is_open()) {
                std::cerr << "Error: Unable to open file CARS.csv" << std::endl;
                return false;
            }

            std::string Car_Registration, Car_Name, Car_Availability, Car_Damage, Car_Holder, Car_Rent, Car_Rented_Date, Car_Reqd_Score;
            
            std::cout << "Enter Car Registration: ";
            std::getline(std::cin, Car_Registration);

            // Check if the Car Registration is unique
            if (!IsCarRegistrationUnique(Car_Registration)) {
                std::cout << "Car Registration already exists. Cannot add duplicate record." << std::endl;
                CARS.close();
                return false;
            }

            // Continue with other inputs...
            
            std::cout << "Enter Car Name: ";
            std::getline(std::cin, Car_Name);

            // while(1){
                // std::cout << "Enter Car Availability: ";
                // std::getline(std::cin, Car_Availability);

                // if(Car_Availability=="T" ){
                    Car_Availability="T";
                    Car_Rented_Date="NULL";
                    Car_Holder="NULL";
                    // break;
                // }
                // else if(Car_Availability=="F"){
                //     std::cout << "Enter Car Holder ID: ";
                //     std::getline(std::cin, Car_Holder);

                //     std::cout << "Enter Car Rented Date: ";
                //     Car_Rented_Date=convertToDate();
                //     break;
                // }
            //     else{
            //         cout<<"Invalid Entry, try again."<<endl;  
            //         continue;
            //     }
            // }

            while(1){
                std::cout << "Enter Car Damage: ";
                std::getline(std::cin, Car_Damage);
                if(stointeger(Car_Damage)<0 || stointeger(Car_Damage)>100){
                    cout<<"Invalid Entry. Try again."<<endl;
                    continue;
                }
                else{
                    Car_Damage=to_string(stointeger(Car_Damage));
                    break;
                }
            }
            
            while(1){
                std::cout << "Enter Car Rental Rate(per day): ";
                std::getline(std::cin, Car_Rent);
                if(stointeger(Car_Rent)<0){
                    cout<<"!!Invalid Entry. Enter Rate again.!!"<<endl;
                    continue;
                }
                else{
                    Car_Rent=to_string(stointeger(Car_Rent));
                    break;
                }
            }
            
            while(1){
                std::cout << "Enter Car Required Score to Rent this Car: ";
                std::getline(std::cin, Car_Reqd_Score);
                if(stointeger(Car_Reqd_Score)<0 || stointeger(Car_Reqd_Score)>100){
                    cout<<"Invalid Entry. Try again."<<endl;
                    continue;
                }
                else{
                    Car_Reqd_Score=to_string(stointeger(Car_Reqd_Score));
                    break;
                }

            }
            

            // Write the entered information into the file in CSV format
            CARS << Car_Registration << ","
                << Car_Name << ","
                << Car_Availability << ","
                << Car_Damage << ","
                << Car_Holder << ","
                << Car_Rent << ","
                << Car_Rented_Date << ","
                << Car_Reqd_Score << "\n";

            CARS.close();
            std::cout << "Car record added successfully." << std::endl;
            return true;
        }
        

        bool Delete_Car_Record() {
            std::string recordID;
            std::cout << "Enter the Car Registration Number to Delete: ";
            std::cin >> recordID;

            std::ifstream inFile("CARS.csv");
            if (!inFile.is_open()) {
                std::cerr << "Unable to open file: CARS.csv" << std::endl;
                return false;
            }

            std::ofstream outFile("CARS_temp.csv");
            if (!outFile.is_open()) {
                std::cerr << "Unable to create temporary file." << std::endl;
                inFile.close();
                return false;
            }

            std::string line;
            bool recordFound = false;

            while (std::getline(inFile, line)) {
                std::istringstream iss(line);
                std::vector<std::string> fields;

                std::string field;
                while (std::getline(iss, field, ',')) {
                    fields.push_back(field);
                }

                if (!fields.empty() && fields[0] == recordID && fields[2] == "T" ) {
                    // Skip writing this line to the temporary file (effectively deleting the record)
                    recordFound = true;
                    continue;
                }
                else if(fields[0]==recordID && fields[2]=="F"){
                    cout<<"Car is rented. Cannot delete the record:"<<endl;
                    inFile.close();
                    outFile.close();
                    std::remove("CARS_temp.csv");
                    
                    return false;
                }

                // Write the unmodified line to the temporary file
                for (size_t i = 0; i < fields.size(); ++i) {
                    outFile << fields[i];
                    if (i < fields.size() - 1) {
                        outFile << ',';
                    }
                }
                outFile << '\n';
            }

            inFile.close();
            outFile.close();

            if (!recordFound) {
                std::cout << "Record with ID " << recordID << " not found." << std::endl;
                std::remove("CARS_temp.csv");
                return false;
            }
            else{
                cout<<"Record Deleted Successfully."<<endl;
            }

            // Rename the temporary file to replace the original file
            std::remove("CARS.csv");
            std::rename("CARS_temp.csv", "CARS.csv");

            return true;
}

        bool Update_Car_Record() {
            string recordID;
            cout << "Enter the Car Registration Number to Update: ";
            cin >> recordID;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');

            ifstream inFile(Cars_Database_Path);
            if (!inFile.is_open()) {
                cerr << "Unable to open file: CARS.csv" << endl;
                return false;
            }

            ofstream outFile("CARS_temp.csv");
            if (!outFile.is_open()) {
                cerr << "Unable to create temporary file." << endl;
                inFile.close();
                return false;
            }

            string line;
            bool recordUpdated = false;  // Flag to track if the record was updated
            bool recordFound = false;  // Flag to track if the record was found

            while (getline(inFile, line)) {
                istringstream iss(line);
                vector<string> fields;

                string field;
                while (getline(iss, field, ',')) {
                    fields.push_back(field);
                }
                if (!fields.empty() && fields[0] == recordID && fields.size() > 2){
                    recordFound = true;
                    if ( fields[2] == "T") {
                        recordUpdated = true;
                        string choice;
                        cout << "Which field do you want to update? (Enter 1 for Car Model, 2 for Car Damage, 3 for Car Rent, 4 for Maximum Permissible Score): ";
                        cin >> choice;
                        cin.ignore(numeric_limits<streamsize>::max(), '\n');

                        if (choice=="1") {
                                cout<<"Enter the new Car Model: ";
                                std::getline(std::cin, fields[1]);
                        }
                        else if(choice=="2"){
                            while(1){
                                cout<<"Enter the Car Damage between 0 and 100.";
                                cin>>fields[3];
                                if(stointeger(fields[3])>=0 && stointeger(fields[3])<=100){
                                    fields[3]=to_string(stointeger(fields[3]));
                                    break;
                                }
                                else{
                                    cout<<"Invalid Input. Please enter a number between 0 and 100."<<endl;
                                }

                            }
                        } 
                        else if(choice=="3"){
                            while(1){
                                cout<<"Enter the new Car Rent per day: ";
                                cin>>fields[5];
                                if(stointeger(fields[5])>=0){
                                    fields[5]=to_string(stointeger(fields[5]));
                                    break;
                                }
                                else{
                                    cout<<"Invalid Input. Please enter a number greater than or equal to 0."<<endl;
                                }
                            }
                        }
                        else if(choice=="4"){
                            while(1){
                                cout<<"Enter the new Required Score.";
                                cin>>fields[7];
                                if(stointeger(fields[7])>=0 && stointeger(fields[7])<=100){
                                    fields[7]=to_string(stointeger(fields[7]));
                                    break;
                                }
                                else{
                                    cout<<"Invalid Input. Please enter a number between 0 and 100."<<endl;
                                }
                            }
                        }
                        else {
                            cout << "Invalid choice. No fields updated." << endl;
                        }
                    }

                    else {
                        // Print a message for records with the 3rd field "F"
                        cout << "This Car is currently Rented Out. Please wait for it to be returned before updating details." << endl;

                    }
                }
            

                // Write the modified or unmodified line to the temporary file
                for (size_t i = 0; i < fields.size(); ++i) {
                    outFile << fields[i];
                    if (i < fields.size() - 1) {
                        outFile << ',';
                    }
                }
                outFile << '\n';
            }

            inFile.close();
            outFile.close();

            if (!recordFound) {
                cout << "Record with ID " << recordID << " not found." << endl;
                remove("CARS_temp.csv");  // Remove the temporary file if no record was found
                return false;
            }
            else if(recordUpdated){
                cout << "Record updated successfully." << endl;
            }
            else{
                cout << "Record not updated." << endl;
            }

            // Rename the temporary file to replace the original file
            remove(Cars_Database_Path.c_str());
            rename("CARS_temp.csv", Cars_Database_Path.c_str());

            

            return true;
        }

        bool Read_Customers(){
            ifstream CUSTOMERS;

            CUSTOMERS.open(Customers_Database_Path);
            if (!CUSTOMERS.is_open())
            {
                cout << "Error: File Open" << endl;
                return 1;
            }
            else if (CUSTOMERS.fail()){
                cerr << "Error: Unable to open file " << Customers_Database_Path << endl;
                return 1;
            }
            else
            {
                cout << "File"<< Customers_Database_Path<< "Opened Successfully" << endl;
            }

            while (CUSTOMERS.peek() != EOF)
            {
                string line;
                getline(CUSTOMERS, line, ',');
                if (line=="\n"){
                    cout << endl;
                }
                else
                {
                cout << line << ' ';
                }
            }
            cout<<"\n";
            CUSTOMERS.close();
            return 0;
        }

        bool Insert_Customer_Record() {
            std::ofstream CUSTOMERS;
            CUSTOMERS.open(Customers_Database_Path, std::ios::app);

            if (!CUSTOMERS.is_open()) {
                std::cerr << "Error: Unable to open file CUSTOMERS.csv" << std::endl;
                return false;
            }

            std::string Customer_ID, Customer_Name, Customer_Password, Customer_Fine, Customer_Record;
            while(1){
                std::cout << "Enter Customer ID: ";
                std::getline(std::cin, Customer_ID);
                if(Customer_ID.substr(0,1)=="C"){
                    break;
                }
                else{
                    cout<<"ID must begin with C. Try again."<<endl;
                    continue;
                }

            }
            

            // Check if the Customer ID is unique
            if (!IsIDUnique(Customer_ID,Customers_Database_Path)) {
                std::cout << "Customer ID already exists. Cannot add duplicate record." << std::endl;
                CUSTOMERS.close();
                return false;
            }

            
            Customer_Fine = "0";
            Customer_Record = "50";

            // Continue with other inputs...
            std::cout << "Enter Customer Name: ";
            std::getline(std::cin, Customer_Name);

            std::cout << "Enter Customer Password: ";
            std::getline(std::cin, Customer_Password);
            // Write the entered information into the file in CSV format
            CUSTOMERS << Customer_ID << ","
                    << Customer_Name << ","
                    << Customer_Password << ","
                    << Customer_Fine << ","
                    << Customer_Record << "\n";

            CUSTOMERS.close();
            std::cout << "Customer record added successfully." << std::endl;
            return true;
        }
        
        bool Delete_Customer_Record() {
            std::ifstream customersFile(Customers_Database_Path);
            if (!customersFile.is_open()) {
                std::cerr << "Error: Unable to open file CUSTOMERS.csv" << std::endl;
                return false;
            }

            std::ofstream tempFile("CUSTOMERS_temp.csv");
            if (!tempFile.is_open()) {
                std::cerr << "Error: Unable to create a temporary file." << std::endl;
                customersFile.close();
                return false;
            }

            string targetCustomerID;
            cout<<"Enter the Customer ID to Delete: ";
            cin>>targetCustomerID;

            std::string line;
            bool recordDeleted = false;

            while (std::getline(customersFile, line)) {
                std::istringstream iss(line);
                std::string customerID, customerName, customerPassword, customerFine, customerRecord;

                std::getline(iss, customerID, ',');
                std::getline(iss, customerName, ',');
                std::getline(iss, customerPassword, ',');
                std::getline(iss, customerFine, ',');
                std::getline(iss, customerRecord, '\n');

                if (customerID == targetCustomerID  && !IsCustomerIDInCars(targetCustomerID)) {
                    if(customerFine == "0"){
                        recordDeleted = true;
                        continue;
                    }
                    else{
                        std::cout << "Customer has a fine. Cannot delete the record." << std::endl;
                        std::remove("CUSTOMERS_temp.csv");
                        return false;
                    }
                }

                // Write the unmodified line to the temporary file
                tempFile << line << '\n';
            }

            customersFile.close();
            tempFile.close();

            if (!recordDeleted) {
                if(IsCustomerIDInCars(targetCustomerID)){
                    std::cout << "Customer has rented a car. Cannot delete the record." << std::endl;
                }
                else{
                    std::cout << "No matching record found for Customer_ID: " << targetCustomerID << std::endl;
                }
                
                std::remove("CUSTOMERS_temp.csv");
                return false;
            }

            // Replace the original file with the temporary file
            std::remove("CUSTOMERS.csv");
            std::rename("CUSTOMERS_temp.csv", "CUSTOMERS.csv");

            std::cout << "Customer record deleted successfully." << std::endl;
            return true;
}

       bool Update_Customer_Record() {
            string recordID;
            cout << "Enter the Customer ID to Update: ";
            cin >> recordID;

            ifstream inFile(Customers_Database_Path);
            if (!inFile.is_open()) {
                cerr << "Unable to open file: CUSTOMERS.csv" << endl;
                return false;
            }

            ofstream outFile("CUSTOMERS_temp.csv");
            if (!outFile.is_open()) {
                cerr << "Unable to create temporary file." << endl;
                inFile.close();
                return false;
            }

            string line;
            bool recordFound = false;  // Flag to track if the record was found

            while (getline(inFile, line)) {
                istringstream iss(line);
                vector<string> fields;

                string field;
                while (getline(iss, field, ',')) {
                    fields.push_back(field);
                }
                if (!fields.empty() && fields[0] == recordID && fields.size() > 2){
                    recordFound = true;
                    while(1){
                        string choice;
                        cout << "Which field do you want to update? (Enter 1 for Customer Name, 2 for Customer Password, 3 for Customer Fine, 4 for Customer Record): ";
                        cin >> choice;
                        cin.ignore(numeric_limits<streamsize>::max(), '\n');


                        if (choice=="1") {
                                cout<<"Enter the new Customer Name: ";
                                std::getline(std::cin, fields[1]);
                                break;
                        }
                        else if(choice=="2"){
                          
                            cout<<"Enter the new Customer Password: ";
                            std::getline(std::cin, fields[2]);
                            break;
                        } 
                        else if(choice=="3"){
                            while(1){
                                cout<<"Enter the new Customer Fine: ";
                                cin>>fields[3];
                                if(stointeger(fields[3])>=0){
                                    fields[3]=to_string(stointeger(fields[3]));
                                    break;
                                }
                                else{
                                    cout<<"Invalid Input. Please enter a number greater than or equal to 0."<<endl;
                                }
                            }
                            break;
                        }
                        else if(choice=="4"){
                            while(1){
                                cout<<"Enter the new Customer Record Value: ";
                                cin>>fields[4];
                                if(stointeger(fields[4])>=0 && stointeger(fields[4])<=100){
                                    fields[4]=to_string(stointeger(fields[4]));
                                    break;
                                }
                                else{
                                    cout<<"Invalid Input. Please enter a number between 0 and 100."<<endl;
                                }
                            }
                            break;
                        }
                        else {
                            cout << "Invalid choice. Enter a number between 1 and 4." << endl;
                        }    
                    }    
                }
            

                // Write the modified or unmodified line to the temporary file
                for (size_t i = 0; i < fields.size(); ++i) {
                    outFile << fields[i];
                    if (i < fields.size() - 1) {
                        outFile << ',';
                    }
                }
                outFile << '\n';
            }

            inFile.close();
            outFile.close();

            if (!recordFound) {
                cout << "Record with ID " << recordID << " not found." << endl;
                remove("CUSTOMEERS_temp.csv");  // Remove the temporary file if no record was found
                return false;
            }
            else{
                cout << "Record Updated Successfully." << endl;
            }

            // Rename the temporary file to replace the original file
            remove(Customers_Database_Path.c_str());
            rename("CUSTOMERS_temp.csv", Customers_Database_Path.c_str());

            

            return true;
        }

        bool Read_Employees(){
            ifstream EMPLOYEES;

            EMPLOYEES.open(Employees_Database_Path);
            if (!EMPLOYEES.is_open())
            {
                cout << "Error: File Open" << endl;
                return 1;
            }
            else if (EMPLOYEES.fail()){
                cerr << "Error: Unable to open file " << Employees_Database_Path << endl;
                return 1;
            }
            else
            {
                cout << "File "<< Employees_Database_Path<< " Opened Successfully" << endl;
            }

            while (EMPLOYEES.peek() != EOF)
            {
                string line;
                getline(EMPLOYEES, line, ',');
                if (line=="\n"){
                    cout << endl;
                }
                else
                {
                cout << line<<' ';
                }
            }
            cout<<"\n";
            EMPLOYEES.close();
            return 0;
        }
        
        bool Insert_Employee_Record() {
            std::ofstream EMPLOYEES;
            EMPLOYEES.open(Employees_Database_Path, std::ios::app);

            if (!EMPLOYEES.is_open()) {
                std::cerr << "Error: Unable to open file EMPLOYEES.csv" << std::endl;
                return false;
            }

            std::string Employee_ID, Employee_Name, Employee_Password, Employee_Fine, Employee_Record;

             while(1){
                std::cout << "Enter Employee ID: ";
                std::getline(std::cin, Employee_ID);
                if(Employee_ID.substr(0,1)=="E"){
                    break;
                }
                else{
                    cout<<"ID must begin with E. Try again."<<endl;
                    continue;
                }

            }

            // Check if the Employee ID is unique
            if (!IsIDUnique(Employee_ID,Employees_Database_Path)) {
                std::cout << "Employee ID already exists. Cannot add duplicate record." << std::endl;
                EMPLOYEES.close();
                return false;
            }

            
            Employee_Fine = "0";
            Employee_Record = "50";

            // Continue with other inputs...
            std::cout << "Enter Employee Name: ";
            std::getline(std::cin, Employee_Name);

            std::cout << "Enter Employee Password: ";
            std::getline(std::cin, Employee_Password);
            // Write the entered information into the file in CSV format
            EMPLOYEES << Employee_ID << ","
                    << Employee_Name << ","
                    << Employee_Password << ","
                    << Employee_Fine << ","
                    << Employee_Record << "\n";

            EMPLOYEES.close();
            std::cout << "Employee record added successfully." << std::endl;
            return true;
        }
    
        
        bool Delete_Employee_Record() {
            std::ifstream employeesFile(Employees_Database_Path);
            if (!employeesFile.is_open()) {
                std::cerr << "**Error: Unable to open file EMPLOYEES.csv**" << std::endl;
                return false;
            }

            std::ofstream tempFile("EMPLOYEES_temp.csv");
            if (!tempFile.is_open()) {
                std::cerr << "**Error: Unable to create a temporary file.**" << std::endl;
                employeesFile.close();
                return false;
            }

            string targetEmployeeID;
            cout<<"Enter the Employee ID to Delete: ";
            cin>>targetEmployeeID;

            std::string line;
            bool recordDeleted = false;

            while (std::getline(employeesFile, line)) {
                std::istringstream iss(line);
                std::string employeeID, employeeName, employeePassword, employeeFine, employeeRecord;

                std::getline(iss, employeeID, ',');
                std::getline(iss, employeeName, ',');
                std::getline(iss, employeePassword, ',');
                std::getline(iss, employeeFine, ',');
                std::getline(iss, employeeRecord, '\n');

                if (employeeID == targetEmployeeID  && !IsCustomerIDInCars(targetEmployeeID)) {
                    if(employeeFine == "0"){
                        recordDeleted = true;
                        continue;
                    }
                    else{
                        std::cout << "**Employee has a fine. Cannot delete the record.**" << std::endl;
                        std::remove("EMPLOYEES_temp.csv");
                        return false;
                    }
                }

                // Write the unmodified line to the temporary file
                tempFile << line << '\n';
            }

            employeesFile.close();
            tempFile.close();

            if (!recordDeleted) {
                if(IsCustomerIDInCars(targetEmployeeID)){
                    std::cout << "**Employee has rented a car. Cannot delete the record.**" << std::endl;
                }
                else{
                    std::cout << "**No matching record found for Employee_ID: " << targetEmployeeID << "**" << std::endl;
                }
                
                std::remove("EMPLOYEES_temp.csv");
                return false;
            }

            // Replace the original file with the temporary file
            std::remove("EMPLOYEES.csv");
            std::rename("EMPLOYEES_temp.csv", "EMPLOYEES.csv");

            std::cout << "**Employee record deleted successfully.**" << std::endl;
            return true;
}
        bool Update_Employee_Record() {
            string recordID;
            cout << "Enter the Employee ID to Update: ";
            cin >> recordID;

            ifstream inFile(Employees_Database_Path);
            if (!inFile.is_open()) {
                cerr << "Unable to open file: EMPLOYEES.csv" << endl;
                return false;
            }

            ofstream outFile("EMPLOYEES_temp.csv");
            if (!outFile.is_open()) {
                cerr << "Unable to create temporary file." << endl;
                inFile.close();
                return false;
            }

            string line;
            bool recordFound = false;  // Flag to track if the record was found

            while (getline(inFile, line)) {
                istringstream iss(line);
                vector<string> fields;

                string field;
                while (getline(iss, field, ',')) {
                    fields.push_back(field);
                }
                if (!fields.empty() && fields[0] == recordID && fields.size() > 2){
                    recordFound = true;
                    while(1){
                        string choice;
                        cout << "Which field do you want to update? (Enter 1 for Employee Name, 2 for Employee Password, 3 for Employee Fine, 4 for Employee Record): ";
                        cin >> choice;
                        cin.ignore(numeric_limits<streamsize>::max(), '\n');


                        if (choice=="1") {
                                cout<<"Enter the new Employee Name: ";
                                std::getline(std::cin, fields[1]);
                                break;
                        }
                        else if(choice=="2"){
                          
                            cout<<"Enter the new Employee Password: ";
                            std::getline(std::cin, fields[2]);
                            break;
                        } 
                        else if(choice=="3"){
                            while(1){
                                cout<<"Enter the new Employee Fine: ";
                                cin>>fields[3];
                                if(stointeger(fields[3])>=0){
                                    fields[3]=to_string(stointeger(fields[3]));
                                    break;
                                }
                                else{
                                    cout<<"Invalid Input. Please enter a number greater than or equal to 0."<<endl;
                                }
                            }
                            break;
                        }
                        else if(choice=="4"){
                            while(1){
                                cout<<"Enter the new Employee Record Value: ";
                                cin>>fields[4];
                                if(stointeger(fields[4])>=0 && stointeger(fields[4])<=100){
                                    fields[4]=to_string(stointeger(fields[4]));
                                    break;
                                }
                                else{
                                    cout<<"Invalid Input. Please enter a number between 0 and 100."<<endl;
                                }
                            }
                            break;
                        }
                        else {
                            cout << "Invalid choice. Enter a number between 1 and 4." << endl;
                        }    
                    }    
                }
            

                // Write the modified or unmodified line to the temporary file
                for (size_t i = 0; i < fields.size(); ++i) {
                    outFile << fields[i];
                    if (i < fields.size() - 1) {
                        outFile << ',';
                    }
                }
                outFile << '\n';
            }

            inFile.close();
            outFile.close();

            if (!recordFound) {
                cout << "Record with ID " << recordID << " not found." << endl;
                remove("EMPLOYEES_temp.csv");  // Remove the temporary file if no record was found
                return false;
            }
            else{
                cout << "Record Updated Successfully." << endl;
            }

            // Rename the temporary file to replace the original file
            remove(Employees_Database_Path.c_str());
            rename("EMPLOYEE_temp.csv", Employees_Database_Path.c_str());

            

            return true;
        }
};

class Customer: public User{
    public:
        Customer(string name, string id, string pass) : User(name, id, pass) {
            database_path="CUSTOMERS.csv";
        };
        
        bool Return_Car() {
            std::string Car_Reg_No;
            std::cout << "Enter the Car Registration Number to Return: ";
            std::cin >> Car_Reg_No;

            // Open CARS.csv for reading
            std::ifstream carsFile("CARS.csv");
            if (!carsFile.is_open()) {
                std::cerr << "**Error: Unable to open file CARS.csv**" << std::endl;
                return false;
            }

            // Open CUSTOMERS.csv for reading and writing
            std::fstream customersFile("CUSTOMERS.csv");
            if (!customersFile.is_open()) {
                std::cerr << "**Error: Unable to open file CUSTOMERS.csv**" << std::endl;
                carsFile.close();
                return false;
            }

            // Open temporary file for writing
            std::ofstream tempFileCustomers("CUSTOMERS_temp.csv");
            if (!tempFileCustomers.is_open()) {
                std::cerr << "**Error: Unable to create temporary customers file.**" << std::endl;
                carsFile.close();
                customersFile.close();
                return false;
            }

            // Open temporary file for writing
            std::ofstream tempFileCars("CARS_temp.csv");
            if (!tempFileCars.is_open()) {
                std::cerr << "**Error: Unable to create temporary cars file.**" << std::endl;
                carsFile.close();
                customersFile.close();
                return false;
            }


            std::string line;
            bool carFound = false;

            while (std::getline(carsFile, line)) {
                std::istringstream iss(line);
                std::string currentCarRegNo, CarModel, CarAvailable, CarDamage, CarRentedtoID, CarRent, CarRentedDate, CarRequiredScore;

                // Assuming Car_Reg_No is the first field in CARS.csv
                std::getline(iss, currentCarRegNo, ',');
                std::getline(iss, CarModel, ',');
                std::getline(iss, CarAvailable, ',');
                std::getline(iss, CarDamage, ',');
                std::getline(iss, CarRentedtoID, ',');
                std::getline(iss, CarRent, ',');
                std::getline(iss, CarRentedDate, ',');
                std::getline(iss, CarRequiredScore, '\n');

                if (currentCarRegNo == Car_Reg_No && CarAvailable == "F" && CarRentedtoID==ID) {
                    // Car_Reg_No exists in CARS.csv and is currently rented to Customer with ID

                    // Take input for return day and damage%
                    string return_date;
                    string damage_percentage;
                    int days_late;

                    
                    while(1){
                        std::cout << "Enter the return date: "<<endl;
                        return_date=convertToDate();

                        days_late=dayDifference(CarRentedDate,return_date);
                        if(days_late>=0){
                            break;
                        }
                        else{
                            std::cout<<"**Invalid Return Date, please enter again.**"<<endl;
                        }
                    }

                    while(1){
                        std::cout << "Enter the damage percentage caused by you (do not include \% sign) : ";
                        std::cin >> damage_percentage;
                        CarDamage=std::to_string(stointeger(CarDamage)+stointeger(damage_percentage));
                        if(stointeger(damage_percentage)>=0 && stointeger(CarDamage)<=100){
                            break;
                        }
                        else{
                            std::cout<<"**Invalid Entry. Please enter a positive number that keeps total damage below 100%**"<<endl;
                        }
                    }
                    
                    
                    

                    // Update CUSTOMERS.csv
                    std::string customerID;
                    while (std::getline(customersFile, line)) {
                        std::istringstream issCustomer(line);
                        std::string currentCustomerID, customerName, customerPassword, customerFine, customerRecord;

                        // Assuming ID is the first field in CUSTOMERS.csv
                        std::getline(issCustomer, currentCustomerID, ',');
                        std::getline(issCustomer, customerName, ',');
                        std::getline(issCustomer, customerPassword, ',');
                        std::getline(issCustomer, customerFine, ',');
                        std::getline(issCustomer, customerRecord, ',');

                        if (currentCustomerID == ID) {
                            // Update the fourth and fifth fields in CUSTOMERS.csv
                            int new_customer_fine = stointeger(customerFine) + 10000*days_late + 100*stointeger(damage_percentage);
                            int new_customer_record = 0.3 * stointeger(customerRecord) +
                                                        0.3 * (days_late * 3.33) +
                                                        0.4 * stointeger(damage_percentage);

                            customerFine=std::to_string(new_customer_fine);
                            customerRecord=std::to_string(new_customer_record);

                        }
                        if(!customerName.empty()){
                            tempFileCustomers << currentCustomerID << ","
                                        << customerName << ","
                                        << customerPassword << ","
                                        << customerFine << ","
                                        << customerRecord << "\n";
                        }
                        else{
                            tempFileCustomers << currentCustomerID <<"\n";
                        }
                    }

                    // Update CARS.csv
                    tempFileCars << currentCarRegNo << ","
                            << CarModel << ","
                            << "T" << ","
                            << CarDamage << ","
                            << "NULL" << ","
                            << CarRent << ","
                            << "NULL" << ","
                            << CarRequiredScore << "\n";
                            

                    carFound = true;
                } else {
                    // Write the unmodified line to the temporary file
                    tempFileCars << line << '\n';
                }
            }

            carsFile.close();
            customersFile.close();
            tempFileCars.close();
            tempFileCustomers.close();

            if (!carFound) {
                // Print a message if the Car_Reg_No is not found in CARS.csv or the car is not currently rented
                std::cout << "**Car with Registration Number " << Car_Reg_No << " is not currently rented or not found in CARS.csv.**" << std::endl;
                std::remove("CUSTOMERS_temp.csv");
                std::remove("CARS_temp.csv");  // Remove the temporary file
                return false;
            }
            else{
                std::cout << "**Car returned successfully.**" << std::endl;
            }

            // Replace the original CUSTOMERS.csv file with the temporary file
            std::remove("CUSTOMERS.csv");
            std::remove("CARS.csv");
            std::rename("CUSTOMERS_temp.csv", "CUSTOMERS.csv");
            std::rename("CARS_temp.csv", "CARS.csv");

            return true;
        }
   
};

class Employee: public User{
    public:
        Employee(string name, string id, string pass) : User(name, id, pass) {
            database_path="EMPLOYEES.csv";
        };

        bool Return_Car() {
            std::string Car_Reg_No;
            std::cout << "Enter the Car Registration Number to Return: ";
            std::cin >> Car_Reg_No;

            // Open CARS.csv for reading
            std::ifstream carsFile("CARS.csv");
            if (!carsFile.is_open()) {
                std::cerr << "Error: Unable to open file CARS.csv" << std::endl;
                return false;
            }

            // Open EMPLOYEES.csv for reading and writing
            std::fstream employeesFile("EMPLOYEES.csv");
            if (!employeesFile.is_open()) {
                std::cerr << "Error: Unable to open file EMPLOYEES.csv" << std::endl;
                carsFile.close();
                return false;
            }

            // Open temporary file for writing
            std::ofstream tempFileEmployees("EMPLOYEES_temp.csv");
            if (!tempFileEmployees.is_open()) {
                std::cerr << "Error: Unable to create temporary employees file." << std::endl;
                carsFile.close();
                employeesFile.close();
                return false;
            }

            // Open temporary file for writing
            std::ofstream tempFileCars("CARS_temp.csv");
            if (!tempFileCars.is_open()) {
                std::cerr << "Error: Unable to create temporary cars file." << std::endl;
                carsFile.close();
                employeesFile.close();
                return false;
            }


            std::string line;
            bool carFound = false;

            while (std::getline(carsFile, line)) {
                std::istringstream iss(line);
                std::string currentCarRegNo, CarModel, CarAvailable, CarDamage, CarRentedtoID, CarRent, CarRentedDate, CarRequiredScore;

                // Assuming Car_Reg_No is the first field in CARS.csv
                std::getline(iss, currentCarRegNo, ',');
                std::getline(iss, CarModel, ',');
                std::getline(iss, CarAvailable, ',');
                std::getline(iss, CarDamage, ',');
                std::getline(iss, CarRentedtoID, ',');
                std::getline(iss, CarRent, ',');
                std::getline(iss, CarRentedDate, ',');
                std::getline(iss, CarRequiredScore, '\n');

                if (currentCarRegNo == Car_Reg_No && CarAvailable == "F" && CarRentedtoID==ID) {
                    // Car_Reg_No exists in CARS.csv and is currently rented to Customer with ID

                    // Take input for return day and damage%
                    string return_date;
                    int days_late;
                    string damage_percentage;

                    while(1){
                        std::cout << "Enter the return date: "<<endl;
                        return_date=convertToDate();

                        days_late=dayDifference(CarRentedDate,return_date);
                        if(days_late>=0){
                            break;
                        }
                        else{
                            std::cout<<"**Invalid Return Date, please enter again.**"<<endl;
                        }
                    }

                    while(1){
                        std::cout << "Enter the damage percentage caused by you (do not include \% sign) : ";
                        std::cin >> damage_percentage;
                        CarDamage=std::to_string(stointeger(CarDamage)+stointeger(damage_percentage));
                        if(stointeger(damage_percentage)>=0 && stointeger(CarDamage)<=100){
                            break;
                        }
                        else{
                            std::cout<<"**Invalid Entry. Please enter a positive number that keeps total damage below 100%**"<<endl;
                        }
                    }

                    // Update EMPLOYEES.csv
                    std::string employeeID;
                    while (std::getline(employeesFile, line)) {
                        std::istringstream issEmployee(line);
                        std::string currentEmployeeID, employeeName, employeePassword, employeeFine, employeeRecord;

                        // Assuming ID is the first field in CUSTOMERS.csv
                        std::getline(issEmployee, currentEmployeeID, ',');
                        std::getline(issEmployee, employeeName, ',');
                        std::getline(issEmployee, employeePassword, ',');
                        std::getline(issEmployee, employeeFine, ',');
                        std::getline(issEmployee, employeeRecord, ',');

                        if (currentEmployeeID == ID) {
                            

                            // Update the fourth and fifth fields in CUSTOMERS.csv
                            int new_employee_fine = stointeger(employeeFine) + 10000*days_late + 100*stointeger(damage_percentage);
                            int new_employee_record = 0.3 * stointeger(employeeRecord) +
                                                        0.3 * (days_late * 3.33) +
                                                        0.4 * stointeger(damage_percentage);

                            employeeFine=std::to_string(new_employee_fine);
                            employeeRecord=std::to_string(new_employee_record);

                        }
                        if(!employeeFine.empty()){
                            tempFileEmployees << currentEmployeeID << ","
                                        << employeeName << ","
                                        << employeePassword << ","
                                        << employeeFine << ","
                                        << employeeRecord << "\n";
                        }
                        else{
                            tempFileEmployees << currentEmployeeID <<"\n";
                        }
                    }

                    // Update CARS.csv
                    tempFileCars << currentCarRegNo << ","
                            << CarModel << ","
                            << "T" << ","
                            << CarDamage << ","
                            << "NULL" << ","
                            << CarRent << ","
                            << "NULL" << ","
                            << CarRequiredScore << "\n";
                            

                    carFound = true;
                } else {
                    // Write the unmodified line to the temporary file
                    tempFileCars << line << '\n';
                }
            }

            carsFile.close();
            employeesFile.close();
            tempFileCars.close();
            tempFileEmployees.close();

            if (!carFound) {
                // Print a message if the Car_Reg_No is not found in CARS.csv or the car is not currently rented
                std::cout << "Car with Registration Number " << Car_Reg_No << " is not currently rented or not found in CARS.csv." << std::endl;
                std::remove("EMPLOYEES_temp.csv");
                std::remove("CARS_temp.csv");  // Remove the temporary file
                return false;
            }
            else{
                std::cout << "Car returned successfully." << std::endl;
            }

            // Replace the original EMPLOYEES.csv file with the temporary file
            std::remove("EMPLOYEES.csv");
            std::remove("CARS.csv");
            std::rename("EMPLOYEES_temp.csv", "EMPLOYEES.csv");
            std::rename("CARS_temp.csv", "CARS.csv");

            return true;
        }

};

int Manager::Manager_Count = 0;

bool CheckCustomerCredentials(const std::string& inputID, const std::string& inputPassword, string CustomerName) {
    // Open CUSTOMERS.csv for reading
    std::ifstream customersFile(Customers_Database_Path);
    if (!customersFile.is_open()) {
        std::cerr << "**Error: Unable to open file CUSTOMERS.csv**" << std::endl;
        return false;
    }

    std::string line;
    while (std::getline(customersFile, line)) {
        std::istringstream iss(line);
        std::string currentID, customerName, currentPassword, customerFine, customerRecord;

        // Assuming ID is the first field and password is the third field in CUSTOMERS.csv
        std::getline(iss, currentID, ',');        // First field (ID)
        std::getline(iss, customerName, ',');     // Second field (Name)
        std::getline(iss, currentPassword, ',');  // Third field (Password)

        // Check if the provided ID and password match any record
        if (currentID == inputID && currentPassword == inputPassword) {
            std::cout << "**Login successful. Welcome, " << customerName << "!**" << std::endl;
            CustomerName=customerName;
            customersFile.close();
            return true;
        }
    }

    std::cout << "**Invalid ID or password. Login failed.**" << std::endl;
    customersFile.close();
    return false;
}

bool CheckEmployeeCredentials(const std::string& inputID, const std::string& inputPassword, string EmployeeName) {
    // Open employees.csv for reading
    std::ifstream employeesFile(Employees_Database_Path);
    if (!employeesFile.is_open()) {
        std::cerr << "**Error: Unable to open file EMPLOYEES.csv**" << std::endl;
        return false;
    }

    std::string line;
    while (std::getline(employeesFile, line)) {
        std::istringstream iss(line);
        std::string currentID, employeeName, currentPassword, employeeFine, employeeRecord;

        // Assuming ID is the first field and password is the third field in CUSTOMERS.csv
        std::getline(iss, currentID, ',');        // First field (ID)
        std::getline(iss, employeeName, ',');     // Second field (Name)
        std::getline(iss, currentPassword, ',');  // Third field (Password)

        // Check if the provided ID and password match any record
        if (currentID == inputID && currentPassword == inputPassword) {
            std::cout << "**Login successful. Welcome, " << employeeName << "!**" << std::endl;
            EmployeeName=employeeName;
            employeesFile.close();
            return true;
        }
    }

    std::cout << "**Invalid ID or password. Login failed.**" << std::endl;
    employeesFile.close();
    return false;
}
int main()
{   
    while(1){
        string choice;

        cout<<"\t"<<"\t"<<"***Welcome to Car Rental System***"<<endl;
        cout<<"1. Manager"<<endl;
        cout<<"2. Customer"<<endl;
        cout<<"3. Employee"<<endl;
        cout<<"4. Exit"<<endl;
        cout<<"**Enter your choice from 1,2,3 or 4:**";
        cin>>choice;
        cin.ignore(numeric_limits<streamsize>::max(),'\n');
        
        if(choice=="1"){
            string ID,password;

            cout<<"\t"<<"\t"<<"**Manager Interface**"<<endl;
            cout<<"Enter Manager ID: ";
            cin>>ID;
            cout<<"Enter Password: ";
            cin>>password;
            if(ID=="M001" && password=="1234"){
                cout<<"**Welcome, John!**"<<endl;
                Manager manager;
                while(1){
                    string choice;
                    cout<<"1. Insert Car Record"<<endl;
                    cout<<"2. Read Car Records"<<endl;
                    cout<<"3. Update Car Record"<<endl;
                    cout<<"4. Delete Car Record"<<endl;
                    cout<<"5. Insert Customer Record"<<endl;
                    cout<<"6. Read Customer Records"<<endl;
                    cout<<"7. Update Customer Record"<<endl;
                    cout<<"8. Delete Customer Record"<<endl;
                    cout<<"9. Read Employee Records"<<endl;
                    cout<<"10. Insert Employee Record"<<endl;
                    cout<<"11. Delete Employee Record"<<endl;
                    cout<<"12. Update Employee Record"<<endl;
                    cout<<"13. Exit"<<endl;
                    cout<<"**Enter your choice from 1 to 13: **";
                    std::cin>>choice;
                    cin.ignore(numeric_limits<streamsize>::max(),'\n');
                    if(choice=="1"){
                        cout<<"**Inserting Car Record**"<<endl;
                        manager.Insert_Car_Record();
                    }
                    else if(choice=="2"){
                        cout<<"**Reading Car Records**"<<endl;
                        manager.Read_CARS();
                    }

                    else if(choice=="3"){
                        cout<<"**Updating Car Record**"<<endl;
                        manager.Update_Car_Record();
                    }
                    else if(choice=="4"){
                        cout<<"**Deleting Car Record**"<<endl;
                        manager.Delete_Car_Record();
                    }
                    else if(choice=="5"){
                        cout<<"**Inserting Customer Record**"<<endl;
                        manager.Insert_Customer_Record();
                    }
                    else if(choice=="6"){
                        cout<<"**Reading Customer Records**"<<endl;
                        manager.Read_Customers();
                    }
                    else if(choice=="7"){
                        cout<<"**Updating Customer Record**"<<endl;
                        manager.Update_Customer_Record();
                    }
                    else if(choice=="8"){
                        cout<<"**Deleting Customer Record**"<<endl;
                        manager.Delete_Customer_Record();
                    }
                    else if(choice=="9"){
                        cout<<"**Reading Employee Records**"<<endl;
                        manager.Read_Employees();
                    }
                    else if(choice=="10"){
                        cout<<"**Inserting Employee Record**"<<endl;
                        manager.Insert_Employee_Record();
                    }
                    else if(choice=="11"){
                        cout<<"**Deleting Employee Record**"<<endl;
                        manager.Delete_Employee_Record();
                    }
                    else if(choice=="12"){
                        cout<<"**Updating Employee Record**"<<endl;
                        manager.Update_Employee_Record();
                    }
                    else if(choice=="13"){
                        cout<<"**Exiting Manager Interface...**"<<endl<<endl;
                        break;
                    }
                    else{
                        cout<<"**Invalid Choice, Please try again.**"<<endl;
                    
                    }
                }
            }
            else{
                cout<<"**Invalid Credentials, Please try again.**"<<endl;
                continue;
            
            }
        }
       
        else if(choice=="2"){
            string ID,password,name;

            cout<<"\t"<<"\t"<<"**Customer Interface**"<<endl;
            cout<<"Enter Customer ID: ";
            cin>>ID;
            cout<<"Enter Password: ";
            cin>>password;

            if(CheckCustomerCredentials(ID,password,name)){
                Customer customer(name,ID,password);
                while(1){
                    string choice;
                    cout<<"1. Rent Car"<<endl;
                    cout<<"2. Return Car"<<endl;
                    cout<<"3. Read Cars Rented by you"<<endl;
                    cout<<"4. Read Available Cars"<<endl;
                    cout<<"5. Exit"<<endl;
                    cout<<"**Enter your choice from 1 to 5: **"<<endl;
                    std::cin>>choice;
                    cin.ignore(numeric_limits<streamsize>::max(),'\n');
                    if(choice=="1"){
                        cout<<"**Renting Car**"<<endl;
                        customer.Rent_Car();
                    }
                    else if(choice=="2"){
                        cout<<"**Returning Car**"<<endl;
                        customer.Return_Car();
                    }
                    else if(choice=="3"){
                        cout<<"**Reading Cars Rented by you**"<<endl;
                        customer.Read_Rented_CARS();
                    }
                    else if(choice=="4"){
                        cout<<"**Reading Available Cars**"<<endl;
                        customer.Read_CARS();
                    }
                    else if(choice=="5"){
                        cout<<"**Exiting Customer Interface...**"<<endl<<endl;
                        break;
                    }
                    else{
                        cout<<"**Invalid Choice, Please try again.**"<<endl;
                    }
                }

            }
        }

        else if(choice=="3"){
            string ID,password,name;

            cout<<"\t"<<"\t"<<"**Employee Interface**"<<endl;
            cout<<"Enter Employee ID: ";
            cin>>ID;
            cout<<"Enter Password: ";
            cin>>password;

            if(CheckEmployeeCredentials(ID,password,name)){
                Employee employee(name,ID,password);
                while(1){
                    string echoice;
                    cout<<"1. Rent Car"<<endl;
                    cout<<"2. Return Car"<<endl;
                    cout<<"3. Read Cars Rented by you"<<endl;
                    cout<<"4. Read Available Cars"<<endl;
                    cout<<"5. Exit"<<endl;
                    cout<<"**Enter your choice from 1 to 5: **";
                    std::cin>>echoice;
                    cin.ignore(numeric_limits<streamsize>::max(),'\n');
                    if(echoice=="1"){
                        cout<<"**Renting Car**"<<endl;
                        employee.Rent_Car();
                    }
                    else if(echoice == "2"){
                        cout<<"**Returning Car**"<<endl;
                        employee.Return_Car();
                    }
                    else if(echoice=="3"){
                        cout<<"**Reading Cars Rented by you**"<<endl;
                        employee.Read_Rented_CARS();
                    }
                    else if(echoice=="4"){
                        cout<<"**Reading Available Cars**"<<endl;
                        employee.Read_CARS();
                    }
                    else if(echoice=="5"){
                        cout<<"**Exiting Employee Interface...**"<<endl<<endl;
                        break;
                    }
                    else{
                        cout<<"**Invalid Choice, Please try again.**"<<endl;
                    }
                }

            }
            
        }

        else if(choice=="4"){
            cout<<"\t"<<"\t"<<"**Thank You For using Car Rental System, please visit again!**"<<endl;
            break;
        }
        
        else{
            cout<<"**Invalid Choice, Please try again.**"<<endl;
            continue;
        }

        
    } 
    return 0;
}

