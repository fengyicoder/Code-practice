
class StringValue;
template<T>
class RCPtr;

class String
{
public:
    class CharProxy {
    public:
        CharProxy(String& str, int index):
            theString(str), charIndex(index) {}
        CharProxy& operator=(const CharProxy& rhs) {
            if (theString.value->isShared()) {
                theString.value = new StringValue(theString.value->data);
            }
            theString.value->data[charIndex] = rhs.theString.value->data[rhs.charIndex];
            return *this;
        }
        CharProxy& operator=(char c) {
            if (theString.value->isShared()) {
                theString.value = new StringValue(theString.value->data);
            }
            theString.value->data[charIndex] = c;
            return *this;
        }
        operator char() const {
            return theString.value->data[charIndex];
        }
        const char* operator&() const
        {
            return &(theString.value->data[charIndex]);
        }
        char* operator&() {
            if (theString.value->isShared())
            {
                theString.value = new StringValue(theString.value->data);
            }
            theString.value->markUnshareable();
            return &(theString.value->data[charIndex]);
        }
    private:
        String& theString;
        int charIndex;
    };
    
    const CharProxy operator[](int index) const {
        return CharProxy(const_cast<String&>(*this), index);
    }
    CharProxy operator[](int index) {
        return CharProxy(*this, index);
    }
    friend class CharProxy;
private:
    RCPtr<StringValue> value;
};