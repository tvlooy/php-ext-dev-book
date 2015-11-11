# Sample Books

## Why Sample Books? {#why-sample-books}

Leanpub lets you create a sample book, which readers can download for free to help them decide whether they want to buy the real book.

To do this, you create a file called Sample.txt in the same folder as this file and the Book.txt file.  Note that, like Book.txt, the Sample.txt file is just a list of files.  It's supposed to be a smaller list than what is in Book.txt, or else you're giving your whole book away!  If you don't want a sample book, just don't create the Sample.txt file, or delete it if you already created it.

## Consequences for Manuscripts

One consequence of this approach to specifying a sample book is that it's an "all or nothing" decision to include a file.  If you include a file, all the chapters and sections in it are included.  Because of this, we recommend a couple things:

1. Either use one chapter per file, or one section per file.
2. Never let a section span multiple files.

These two guidelines mean that your sample book is a lot less likely to have problems.  If you don't follow them, things can still work, but you can sometimes create a situation where your book will be created fine but your sample book will not.  Basically, when we create a book or a sample book, we stick all the content in all the included files together.  If you do things like include a section (sections start with ##) but not the chapter that contains it (chapters start with #) your book will look a bit funny.

So, if you use the "one chapter per file" approach, things will just work.  If you use the "one section per file" approach, make sure you include the first section in every chapter.
