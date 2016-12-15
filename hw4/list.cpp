
void listAll(const Class* c, string path)  // two-parameter overload
{
    
    path += c->name();
    
    cout << path << endl;
    
    std::vector<Class*>::const_iterator it = c->subclasses().begin();
    for (; it != c->subclasses().end(); it++)
    {
        listAll(*it, path+"=>");
    }
    
}