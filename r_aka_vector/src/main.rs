use::std::alloc;
use std::alloc::Layout;
use::std::ptr::NonNull;
use::std::marker::PhantomData;


pub struct Vector<T> {
    ptr: NonNull<T>,
    len: usize,
    cap: usize,
    _marker: PhantomData<T>
}

impl<T> Vector<T> {
    pub fn new() -> Self {
        Self { 
            ptr: NonNull::dangling(),
            len: 0,
            cap: 0,
            _marker: PhantomData
        }
    }

    #[inline]
    pub fn cap(&self) -> usize { self.cap }

    #[inline]
    pub fn len(&self) -> usize { self.len }

    #[inline]
    pub fn empty(&self) -> bool { self.len == 0 }

    pub fn with_capacity(capacity: usize) -> Self {
        if capacity == 0 {
            return Self::new();
        }

        let layout = Layout::array::<T>(capacity).expect("capacity error");

        let raw_ptr = unsafe { alloc::alloc(layout) };

        let ptr = match NonNull::new(raw_ptr.cast::<T>()) {
            Some(p) => p,
            None => alloc::handle_alloc_error(layout)
        };

        Self {
            ptr,
            len: 0, 
            cap: capacity,
            _marker: PhantomData
        }
    }
}

impl<T> Drop for Vector<T> {
    fn drop(&mut self) {
        if self.cap == 0 { return; }

        unsafe {
            for i in (0..self.len).rev() {
                self.ptr.as_ptr().add(i).drop_in_place();
            }

            let layout = Layout::array::<T>(self.cap).expect("capacity error");
            alloc::dealloc(self.ptr.as_ptr().cast(), layout);
        }
    }
}

fn main() {
    println!("Hello, world!");
}
