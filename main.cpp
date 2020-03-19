#include <iostream>
#include <vector>
#include <string>

#include <dlib/clustering.h>
#include <dlib/rand.h>

using namespace std;
using namespace dlib;

typedef matrix<double, 2, 1> sample_type;

typedef radial_basis_kernel<sample_type> kernel_type;

void generate_samples(const long num) {
    std::vector<sample_type> s;
    sample_type m;
    dlib::rand rnd;
  
    // make some samples near the origin
    double radius = 0.5;
    for (long i = 0; i < num; ++i)
    {
        double sign = 1;
        if (rnd.get_random_double() < 0.5)
            sign = -1;
        m(0) = 2 * radius * rnd.get_random_double() - radius;
        m(1) = sign * sqrt(radius * radius - m(0) * m(0));

        // add this sample to our set of samples we will run k-means 
        s.push_back(m);
    }

    // make some samples in a circle around the origin but far away
    radius = 10.0;
    for (long i = 0; i < num; ++i)
    {
        double sign = 1;
        if (rnd.get_random_double() < 0.5)
            sign = -1;
        m(0) = 2 * radius * rnd.get_random_double() - radius;
        m(1) = sign * sqrt(radius * radius - m(0) * m(0));

        // add this sample to our set of samples we will run k-means 
        s.push_back(m);
    }

    // make some samples in a circle around the point (25,25) 
    radius = 4.0;
    for (long i = 0; i < num; ++i)
    {
        double sign = 1;
        if (rnd.get_random_double() < 0.5)
            sign = -1;
        m(0) = 2 * radius * rnd.get_random_double() - radius;
        m(1) = sign * sqrt(radius * radius - m(0) * m(0));

        // translate this point away from the origin
        m(0) += 25;
        m(1) += 25;

        // add this sample to our set of samples we will run k-means 
        s.push_back(m);
    }

    std::ofstream file;
    file.open("Kkmeans_example.txt");
    for (auto& a : s) {
        file << a(0) << ";" << a(1) << "\n";
    }

}


void generate_starsky(const long num) {
    std::vector<sample_type> s;
     sample_type m;
    dlib::rand rnd;
    
    for (size_t i = 0; i < num; ++i)
    {
        m(0) = 100 * rnd.get_random_double() * ((rnd.get_random_double() < 0.5) ? -1 : 1);
        m(1) = 100 * rnd.get_random_double() * ((rnd.get_random_double() < 0.5) ? -1 : 1);
        s.push_back(m);
    }

    std::ofstream file;
    file.open("Kkmeans_starsky.txt");
    for (auto& a : s) {
        file << a(0) << ";" << a(1) << "\n";
    }
 }



int main(int args, char ** argv)
{
        
    kcentroid<kernel_type> kc(kernel_type(0.1), 0.01, 8);

    kkmeans<kernel_type> test(kc);

    std::vector<sample_type> samples;
    std::vector<sample_type> initial_centers;

    std::string str;
    sample_type m;

    //генерация файлов входных данных
    // generate_samples(50);
    //generate_starsky(200);

    //ввод данных
    //std::ifstream input;
    //input.open("Kkmeans_example.txt");
    //input.open("Kkmeans_starsky.txt");
   //  while (std::getline(input, str)) {
    while (std::getline(std::cin, str)) {
        auto it = str.find(";");
        m(0) = std::stod(str.substr(0, it));
        m(1) = std::stod(str.substr(it+1));
        samples.push_back(m);
    }

    const long num = samples.size();
 
    test.set_number_of_centers(std::atoi(argv[1]));
    pick_initial_centers(std::atoi(argv[1]), initial_centers, samples, test.get_kernel());
    test.train(samples, initial_centers);

    //вывод результата в файл
    std::ofstream out_file;
    out_file.open("kkmeans_res.txt");
    for (auto& s : samples) {
        out_file << s(0) << ";" << s(1) << ";" << test(s) << "\n";
    }
    out_file.close();
 }
