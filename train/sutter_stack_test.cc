// Нейтральность нужна. Т.к. обобщенный контейнер не знает ничего о типах хр. в нем элементов, а вызывающий код должен знать.

#include <stdexcept>
#include <algorithm>
#include <iostream>

#include <gtest/gtest.h>

using std::logic_error;
using std::cout;

//namespace {}

// safe stack
// Требования к Т довольно строгие.
//   - ..
//   - Безопасное присваивание - ?
//   - конструктор по умолчанию
// TODO: снизить требование
template<class T> 
class Stack
{
public:
  Stack();
  ~Stack();

  // Step 3
  Stack(const Stack&);
  Stack& operator=(const Stack&);
  T* NewCopy(const T* src, size_t srcsize, size_t destsize);

  // Step 4
  size_t Count() const;
  void Push(const T&);
  // решают две задачи
  T Pop_nosafe();  // if empty throw
  void Pop_better_but(T& result);
  // safe
  T& Top();  // TODO: const?
  void Pop();

private:
  T* v_;
  size_t vsize_;
  size_t vused_;  // количество реально исп. объектов
};

// TODO: я не верю что это безопасно! это неуправлемый ресурс же?
template<class T>
Stack<T>::Stack() :
  //http://stackoverflow.com/questions/377178/how-does-the-standard-new-operator-work-in-c
  // "new handles constructor exceptions"
    // TODO: как вызовется delete []?!
    // http://ptgmedia.pearsoncmg.com/imprint_downloads/informit/aw/meyerscddemo/DEMO/MAGAZINE/SU_FRAME.HTM
    v_(/*0 step 2*/new T[10]),  // может сгенерировать и new and T()
    vsize_(10),
    vused_(0) {
  //v_ = new T[vsize_];  // Step 2: лучше передать в списке иниц. Мишко похожее говорил в GTechTalk
  // T::T() may throw

  // но ведь деструктор не полностью сконструированного объекта не вызывается?
  // Похоже при неудаче delete вызывается из new(?)
  //
  // Если T::T() бросит исключение, то его деструктор не вызовется - "вызыв. дестр. созд. объектов"
  // Но деструкторы то членов вызываются?
}
template<class T>
Stack<T>::~Stack() {
  delete [] v_;
  // T::~T() // throw()  // must be!!
}

// Step 3
template<class T>
T* Stack<T>::NewCopy(const T* src, size_t srcsize, size_t destsize) {
  using std::copy;
  assert(destsize >= srcsize);
  T* dest = new T[destsize];
  try {
    copy(src, src+srcsize, dest);  //DANGER: used =,  не ясно в чем проблема, но она есть
  } catch (...) {
    delete [] dest;
    throw;
  }
  return dest;
}

// Step 3: neutral
template<class T>
Stack<T>::Stack(const Stack<T>& other) :
  v_(NewCopy(other.v_,
             other.vsize_,
             other.vsize_)),  // strange
  vsize_(other.vsize_),
  vused_(other.vused_) {
}

// Step 3: assign
template<class T>
Stack<T>& Stack<T>::operator=(const Stack<T>& other) {
  if (this != &other) {
      // may throw
      T* v_new = NewCopy(other.v_, other.vsize_, other.vsize_);

      // must not throw - change state object
      delete [] v_;
      v_ = v_new;
      vsize_ = other.vsize_;
      vused_ = other.vused_;
  }
  return *this;
}

// Step 4
template<class T>
size_t Stack<T>::Count() const  { return vused_; }

template<class T>
void Stack<T>::Push(const T& t) {
  if (vused_ == vsize_) {
    // нужно увеличить память
    // may throw - isolated code
    size_t vsize_new = vsize_*2+1;
    T* v_new = NewCopy(v_, vsize_, vsize_new);

    // no throw
    delete[] v_;
    v_ = v_new;
    vsize_ = vsize_new;
  }
  v_[vused_] = t;  //DANGER: if throw - then no Strong Warr.
    // Итераторы станут не действ.
  ++vused_;  // pointer to next
}

// Step 5
// bad pop
// Проблема не в функции, а в том что такая реализация
// заставляет клиента писать небезопасный код.
//
// !! Функции мутаторы не должны возвр. по значению!!
template<class T>
T Stack<T>::Pop_nosafe() {
  if (vused_ == 0) {
    throw "if empty";
  } else {
    T result = v_[vused_];
    --vused_;
    return result;  // при копировании может возн. искл.
    // объект не дошел до плучателя, а состояние уже изменили
  }
}

template<class T>
void Stack<T>::Pop_better_but(T& result) {
  if (vused_ == 0) {
    throw "if empty";
  } else {
    result = v_[vused_];  // сперва снимаем значение
    --vused_;
    //return result;  // при копировании может возн. искл.
    // объект не дошел до плучателя, а состояние уже изменили
  }
}

template<class T>
T& Stack<T>::Top() {
  if (vused_ == 0) {
    throw "if empty";
  } else {
    return v_[vused_];
  }
}

template<class T>
void Stack<T>::Pop() {
  if (vused_ == 0) {
    throw "if empty";
  } else {
    --vused_;
  }
}

// А ведь и правда удаляет
int i = 0;
class TestThrow {
public:
  TestThrow() { i++;
                if (i == 5)
                  throw std::runtime_error(""); }
  ~TestThrow() {
      cout << "dtr\n";
  }
};


TEST(Sutter, Stackstrange) {
  EXPECT_THROW(Stack<TestThrow>(), std::runtime_error);// stack;
}

namespace util {
template<class T1, class T2>
void construct(T1* p, const T2& value) {
  new (p) T1(value);
}

template<class T>
void destroy(T* t) {
  t->~T();
}

template<class FwdIter>
void destroy(FwdIter first, FwdIter last) {
  while(first != last) {
      destroy(&*first);
      ++first;
  }
}
}

/// Updated Stack - step 6:
// Позволяет снять ограничения на T
// Нужен только конструктор копирования и nothrow ctor
template<class T>
class StackImpl {
/*?*/  // protected - if inh. public - member
protected:
//public:  // тогда можно будет включить членом класса, что много
  //  лучше!!
  StackImpl(size_t size=0);
  ~StackImpl();
  void Swap(StackImpl& other) throw();  // nothrow!! very important

  // видны дочернему - if protected - no
  T* v_;
  size_t vsize_;
  size_t vused_;

private:
  StackImpl(const StackImpl&);
  StackImpl& operator=(const StackImpl&);
};

template<class T>
StackImpl<T>::StackImpl(size_t size)
  : v_(static_cast<T*>
  (size == 0
      ? 0
  : operator new(sizeof(T)*size))),
       vsize_(size),
       vused_(0) {
}

template<class T>
StackImpl<T>::~StackImpl() {
  using util::destroy;

  destroy(v_, v_+vused_);  // compiled but...

  operator delete(v_);
}


template<class T>
void StackImpl<T>::Swap(StackImpl<T>& other) throw() {
  using std::swap;

  swap(v_, other.v_);  // а что будет потом с. Как деструктор определяет сколько раз вызвать дестр. объекта?
  swap(vsize_, other.vsize_);
  swap(vused_, other.vused_);
}

// Жестко связан с классом упр. ресурсами
// p. 125
template<class T>
class StackInh : private StackImpl<T> {
public:
  explicit StackInh(size_t size=0)
    : StackImpl<T>(size) {}

  //~StackInh();  // not need
  StackInh(const StackInh& other) {
    using util::construct;

    // gcc4.7.2 - no compiled without this->
    while (this->vused_ < other.vused_) {
        construct(this->v_+this->vused_, other.v_[this->vused_]);
        ++this->vused_;
    }
  }

  StackInh& operator=(const StackInh& other) {
    // не нужно проверять присв. самому себе
    StackInh temp(other);
    this->Swap(temp);
    return *this;
  }

  size_t Count() const {return this->vused_;}
  void Push(const T& t) {
    if (this->vused_ == this->vsize_) {
        // need reallocation
        StackInh temp(this->vsize_ * 2 +1);
        while (temp.Count() < this->vused_)
          temp.Push(this->v_[temp.Count()]);
        temp.Push(t);
        this->Swap(temp);
    } else {
      using util::construct;
        construct(this->v_ + this->vused_, t);
        ++this->vused_;
    }
  }
  T& Top();
  void Pop();
};

TEST(Sutter, StackFirst) {
  Stack<int> stack;
  StackInh<int> stackInh(10);
  StackInh<int> a(stackInh);
  a.Push(9);
  stackInh = a;
}

namespace esafty {

// если какая-то часть объекта при конструировании сгенерировала исключение, то и весь констр. должен сгенерировать искл.
//   восстанавливать ничего нельзя!!
// 
// Та же фигня с двухфайзным конструированием.
//
// DANGER: Сильно влияет на резработку класса

struct elem {
  explicit elem(int) {}
  ~elem() {
    cout << "elem dtor\n";
  }
};

struct compose {
  compose() : 
    // инициализация членов класса - будут вызваны дестр. только полностью созданных объектов-значений
    // указатели тоже объекты значения, но если класс владеет тем на что указывает указатель, то 
    // при генерации исключения ресурс будет утерян, даже если мы освобождаем его в деструкторе.
    //    деструктор просто не будет вызван - объект не сконструирован.
    e(0), e2(1) 
    // дополнительные действия
    { throw 0; }

  ~compose() {
    cout << "compose dtor\n";
  }

  // FIXME: для value-объектов с конструкторами по умолчанию вызыв дестр?
  elem e;  // деструктор вызовется, 
  elem e2;
  //elem* e_ptr;  // автоматом не разрушится, разрушится только указатель - он значение
};

TEST(Exc, Composite) {
  EXPECT_ANY_THROW(compose());
}

/*
class Widget {
public:
  Widget& operator=(const Widget&);
private:
  // могут бросить искл. оба, как сделать безоп. оператор присв. не изменяя структуру
  // Нужно атомартно изменить оба - не возможно
  //T1 t1_;
  //T2 t2_;

  // V0 - pimpl
};

// "Почти" безопасно - если есть side-effects то изменения не откатить
class Widget {
public:
  Widget& operator=(const Widget& other) {
    Widget tmp(other);  // конструктор копирования
    swap(tmp);
    return *this;
  }

  void swap(Widget& other) {
    // much Move - не факт что сработает std::swap!!
    auto_ptr<WidgetImpl> tmp(pimpl_);  // передаем влядение
    pimpl_ = other.pimpl_;
    other.pimpl_ = tmp;
  }

private:
  // могут бросить искл. оба, как сделать безоп. оператор присв. не изменяя структуру
  // Нужно атомартно изменить оба - не возможно
  

  // V0 - pimpl
  class WidgetImpl;
  auto_ptr<WidgetImpl> pimpl_;
};

class Widget::WidgetImpl {
public:
  //T1 t1_;
  //T2 t2_;
};

/// Есть какая-то хрень и с наследованием!! по сути пример похож на пердыдущий
*/

}