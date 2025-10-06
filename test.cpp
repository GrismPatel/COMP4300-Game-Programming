#include <iostream>
#include <string>
#include <vector>


// class Student {
// private:
//     std::string _firstName    = "First Name";
//     std::string _lastName     = "Last Name";
//     int         _id           = 0;
//     float       _averageScore = 0;
// public:
//     Student();
//     Student(
//         const std::string& firstName,
//         const std::string& lastName,
//         const int& id,
//         const float& averageScore
//     ) :_firstName(firstName), _lastName(lastName), _id(id), _averageScore(averageScore) {};
    
//     const std::string& getFirstName() const {
//         return _firstName;
//     };

//     const std::string& getLastName() const {
//         return _lastName;
//     };

//     const int& getId() const {
//         return _id;
//     };

//     const float& getAverageScore() const {
//         return _averageScore;
//     };

//     void print() const {
//         std::cout << _firstName << " " << _lastName << " " << _id << " " << _averageScore << " " << std::endl;
//     };

//     ~Student() {
//         std::cout << "Derefrencing student." << std::endl;
//     };
// };


// class Course {
// private:
//     std::string _courseName = "Course Name";
//     std::vector<Student> _students;
// public:
//     Course();
//     Course(
//         const std::string& courseName
//     ) :_courseName(courseName) {};

//     void addStudents(const Student& student) {
//         _students.push_back(student);
//     };

//     const std::vector<Student>& getStudents() const {
//         return _students;
//     };
    
//     void print() const {
//         for (auto& student : _students) {
//             student.print();
//         };
//     };

//     ~Course() {
//         std::cout << "Derefrencing Course" << std::endl;
//     };
// };



template <typename T>
class DynamicArray {
private:
    size_t _array_size;
    T *_array;
public:
    DynamicArray(
        size_t array_size
    ) : _array_size(array_size), _array(new T[array_size]) {};
    
    // range based loop does not work because _array is pointer and it does not know the size. 
    // It only works for std::vector when size is known.
    void print() const {
        for (size_t index=0; index < _array_size; index++) {
            std::cout << index << " " << _array[index]  << std::endl;
        };
    };

    const T getValue(const size_t index) const {
        return _array[index];
    }
    
    const void setValue(const size_t index, const T value) {
        _array[index] = value;
    }

    // returns reference of _array[index].
    T& operator [] (const size_t index) {
        return _array[index];
    }

    // this is like a getter. first const so return value cannot be changed,
    // second const so that index cannot be changed,
    // third const so the method does not change anything
    // We add this method so that if someone calls the class like `const DynamicArray variableName(args)`, `variableName[index]` should not fail.
    const T& operator [] (const size_t index) const {
        return _array[index];
    }

    ~DynamicArray() {
        delete [] _array;
        std::cout << "Derefrencing it" << std::endl;
    };
};


int main() {
    // const Student student1("Grism", "Patel", 1, 10.3);
    
    // Course course1("Game");
    // course1.addStudents(student1);
    // course1.print();

    DynamicArray<int> array(5);
    array[1] = 46;
    std::cout << array[1] << std::endl;
    array.print();

    return 0;
};
