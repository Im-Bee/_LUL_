#ifndef LUL_IAPPLICATION_H
#define LUL_IAPPLICATION_H

#define LUL_DECLARE_APPLICATION(appClass) _LUL_::IApplication* _LUL_::EntryApplication() { return new appClass; }

namespace _LUL_
{
    /**
    * Application interface. */
    class LUL_API IApplication
    {
    public:
        IApplication() noexcept = default;
        ~IApplication() noexcept = default;

        IApplication(IApplication&) = delete;
        IApplication(IApplication&&) = delete;

    public:
        virtual void Initialize()	= 0;
        virtual void Update()		= 0;
        virtual void Destroy()		= 0;
    };

    IApplication* EntryApplication();
}

#endif // !LUL_IAPPLICATION_H
