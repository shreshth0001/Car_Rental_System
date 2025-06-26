
# CS253_Car_Rental_System
##Instructions to Run
- ⁠Save the files in a directory, say ⁠ x ⁠.
- ⁠Open your terminal and go to directory ⁠ x ⁠.
- Type the following commands in the terminal:
  
  
⁠   g++ solution.cpp -o runner.exe
  ./runner.exe
   ⁠
- The code will now run
- Make sure all csv files are in x as well, and are named as in repository

## Overview 
- This software allows three types of users to log in: Manager, Customer and Employee.
- The Manager is unique and has credentials (ID,Pwd,Name)=("M001","1234","John")
- The Customers' details(ID,Name,Password,Fine,Record) are all stored in a database "CUSTOMERS.csv"
- The Employees' details(ID,Name,Password,Fine,Record) are all stored in a database "EMPLOYEES.csv"
- Customers and Employees can rent cars for the day via this software. Any late returns and damages caused are accounted for.
- Details of all Cars (Reg No, Model, Availability, Damage, Rented to ID, Rent, Rented Date, Required Score) are stored in CARS.csv
- The Manager Can Read,Insert, Delete, Update any record of any of the three databases. However, he cannot rent or return a car.
- The Customers and Employees can Read Available Cars, Read Cars Rented by them, Rent a Car, and Return a currently rented car. The Employees need to pay 15% lesser than the customers to rent a car.
## Code Structure
### Parent Class: User
##### Protected Attributes:
- ID
- Password
- Name
##### Public Constructor Function
##### Public Virtual Functions
- Read_CARS()
- Read_Rented_CARS()
- Rent_Car()
### Child Class#1 : Manager (public access inheritance)
##### Private Functions
- Checking for duplicacy in Car Registration No.: IsCarRegistrationUnique()
- Checking for Duplicacy in Customer/Employee ID (Database parametrised): IsIDUnique()
- Checking whether given Customer/Employee has rented any car : IsCustomerIDInCars()
##### Constructor Function
##### Public Functions
- Rent Car disabled by overwriting
- Read_CARS()
- Insert_Car_Record()
- Delete_Car_Record()
- Update_Car_Record()
- Read_Customers()
- Insert_Customer_Record()
- Delete_Customer_Record()
- Update_Customer_Record()
- Read_Employees()
- Insert_Employee_Record()
- Delete_Employee_Record()
- Update_Employee_Record()
### Child Class#2 : Customer (public access inheritance)
##### Public Functions
- Constructor
- Return_Car()
 
### Child Class#3 : Employee (public access inheritance)
##### Public Functions
- Constructor
- Return_Car()

## Assumptions
##### Manager Privileges
- Manager can only update/delete cars that are not currently rented out.
- Manager can only delete Customers/Employees that do not currently own cars and have cleared their fines.
- Only the manager can create users, and clear fines.
- Manager cannot alter IDs of different users.
- Manager can change passwords.
##### Customer_Record: 
- Customer_Record is a column in CUSTOMERS.csv that stores a number between 0 and 100, indicating the defaulting rate of that customer(therefore a score of 0 indicates a very reliable customer, whereas a score of 100 indicates a customer who constantly defaults).
- Similarly for EMPLOYEES.csv
- The formula is: Customer Record= 0.3 * Current Value + 0.3* Days Late * 3.33 + 0.4 (Damage%)
- New user holds average record of 50.
##### Fines:
- The Car Rental System Charges a fixed rate as long as car is returned on the same date, any extra days attract a fine of Rs 10,000 per day
- 1% damage attracts a fine of Rs 100 
- The formula is: New_Fine= Original_Fine + 10000*days_late + 100*damage_percentage

##### Rental
###### A car can be rented if:
- It is not currently rented by someone else
- Paid amount is greater than or equal to daily rate(15% discount for employees)
- Record of the consumer is less than or equal to maximum permissible score for the particular car

##### IDs
- The manager has a fixed ID: M001
- All customer IDs start with a C
- All employee IDs start with an E

##### Dates
- Since it will be impractical to test the software with real time dates, this code takes dates as input from the user(customer/employee) in integers and stores in the string format "dd/mm/yyyy" each time they rent and return a car.

##### Car Insertions
- Any record inserted into "CARS.csv" must be available for rent(i.e, the manager cannot insert a car that is rented out already)

##### String to integer conversions
- The code uses a custom function stointeger that assumes input of 0 whenever the user inputs a non digit character in fields that are required to be converted to integers.
- It also filters out initial digits before occurence of non digit(non digits include decimal points) to store an integer value.



## Guide to Using the Software
- All csv files may be opened using excel by setting delimiters as both comma(,) and pipe(|).
- To Log In as manager, use credentials ("M001",1234). Manager's name is John.
- For credentials of Customer and Employee, you can open their respective csv files.


