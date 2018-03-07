// package com.mycompany.app;

interface Builder<T> {
    T build();
}

class Person {
    String name;
    int age;

    public Person(String name, int age) {
        this.name = name;
    }

    public static class PersonBuilder implements Builder<Person> {
        private String name;
        private int age;

        public PersonBuilder(String name) {
            this.name = name;
        }

        public Person build() {
            return new Person(name, age);
        }

        public PersonBuilder setage(int age) {
            this.age = age;
            return this;
        }
    }

    public static PersonBuilder personBuilder(String name) {
        return new PersonBuilder(name);
    }
}
public class temp 
{
    public static void main( String[] args )
    {
        Builder<Person> a = Person.personBuilder("name").setage(50);
        System.out.println("hello");
        
    }
}
