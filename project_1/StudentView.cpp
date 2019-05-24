#include "StudentView.h"
#include "DatabaseModel.h"
#include "StudentInfo.h"
#include <iostream>


void StudentView::menu()
{
    while (true)
    {   resp:
        std::cout << "1 - My marks\n2 - My personal information\n3 - My sucjects\n4 - Back to Login Page\n";
        std::cout << "If you want to leave, press q|exit|quit\n";
        get_response();
        if (m_stop) return;
        //you cannot switch case with strings so first we check if string is length 1 then we switch case on first character
        if (m_response.size() == 1)
        {
            switch (m_response[0])
            {
                case '1':
                    marks_view();
                    break;
                case '2':
                    personal_information();
                    break;
                case '3':
                    subject_view();
                    break;
                case '4':
                {std::string q="q";
                    is_exit_requested(q);
                    return;}
                default:
                {
                    std::cout << "Wrong Input!\n";
                    goto resp;
                }
            }
        }
    }
}





void StudentView::marks_view()
{
    std::list<project::MarkInfo> mark_list;
    mark_list=m_db.get_marks_by_student_id(id);
    std::cout << "\nYour marks:\n";
    for (auto& mark : mark_list)
    {
        std::cout << mark << "\n";
    }
    
    
    /*czy lepiej:
    std::cout << "\nYour marks:\n";
    for (auto& mark : m_db.get_marks_by_student_id(id);)
    {
        std::cout << mark << "\n";
    }*/
}



void StudentView::personal_information()
{
    project::StudentInfo stud_info = m_db.get_by_id<project::StudentInfo>(id);
    std::cout<<"\nYour personal information: "<<std::endl;
    std::cout<<"Name: "<<stud_info.m_firstname<<std::endl;
    std::cout<<"Surname: "<<stud_info.m_lastname<<std::endl;
    std::cout<<"Class ID: "<<stud_info.m_class_id<<std::endl;
    std::cout<<"PESEL: "<<stud_info.m_PESEL<<std::endl;
    std::cout<<"City: "<<stud_info.m_city<<std::endl;
    std::cout<<"Address: "<<stud_info.m_address<<std::endl;
    
}


void StudentView::subject_view()
{
    
    //lista przedmiotow z prowadzacymi - rozumiem to tak ze tworze liste z wszystkimi przedmiotami ktore sa przyporzadkowane do danego ID studenta tylko potrzebuje do tego klasy professor
    
    
    
}


std::string StudentView::zmiana(std::string& ciag)
{ int i=0;
    
    while(ciag[i]!=0)
    {
        if (ciag[i]>96 && ciag[i]<122)
        {
            ciag[i]=(ciag[i]-32);
        }
        i++;
    }
    return ciag;
}
