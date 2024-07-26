#include <napi.h>

class ActualClass {
    public:

    // constructor
    ActualClass(double value);

    // getter for the value
    double getValue();

    //adds the toAdd value to the value
    double add(double toAdd);

    private:

    double value_;
};

class ActualClassWrapped : public Napi::ObjectWrap<ActualClassWrapped> {
    public:

    // Init function for setting the export key to JS
    static Napi::Object Init(Napi::Env env, Napi::Object exports);

    // Constructor to initialise
    ActualClassWrapped(const Napi::CallbackInfo& info);

    private:
    // reference to store the class definition that needs to be exported to JS
    static Napi::FunctionReference constructor;

    //wrapped getValue function
    Napi::Value GetValue(const Napi::CallbackInfo& info);

    //wrapped add function
    Napi::Value Add(const Napi::CallbackInfo& info);

    // Internal instance of actual class used to perform actual operation
    ActualClass *actualClass_;
};
