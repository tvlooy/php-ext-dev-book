# Markdown

## Why Should You Care?

A Leanpub book is composed of a bunch of Markdown files.

The order of these files is defined in a file called Book.txt, which is in the same folder as this file.

To learn about the syntax and philosophy of Markdown, see [this article by John Gruber](http://daringfireball.net/projects/markdown/syntax).

Briefly, Markdown is a nice way of writing content which is easily transformed into HTML.  For example, # at the beginning of a line becomes an h1, ## becomes an h2, ### becomes an h3, etc.  Lists, paragraphs and other formatting is also intuitive.

## How Markdown is Used in Leanpub

Even if you know Markdown, you need to learn a few things about how we use it at Leanpub.

### Heading Levels Become Chapters and Sections

First, we use # chapters, ## for sections and ### for sub-sections.  (You can also use #### for sub-sub-sections, but don't get carried away!  Most technical books are good with just #, ## and ###, and most business and fiction books are good with just # and possibly ##.)

So, if you look at the top of the Markdown.txt file you'll see that it has one #, meaning it is a Chapter.

Another thing this means is that one file can contain as many chapters or sections as you want: every # makes a new chapter; it has nothing to do with what file it is in.  However, we strongly recommend having one file per chapter (or one file per chapter section), since it makes creating sample books easier and keeps your book directory cleaner.  Since we recommend this, that's what we'll do in this example book.

### Links Become Footnotes

We support Markdown syntax for links, as well as normal HTML links.  Both of these are converted into functioning footnotes in the PDF.  Here's an example of a link to [Leanpub](http://leanpub.com).

## Markdown Extensions in Leanpub

We've made a few additions to Markdown for use in Leanpub.  Two of the most important are tables and crosslinks.  Furthermore, since Leanpub is so good for technical books, we also support extensions for external code samples, special directives for code syntnax highlighting, etc.  If this is a technical book, you will see these discussed in the Code Samples chapter.

### Tables

Creating a table in a Leanpub book is relatively simple.  Here's an example:

|First Name | Last Name     | Email             |
|-----------|---------------|-------------------|
|Peter      | Armstrong     | peter@leanpub.com |
|Scott      | Patten        | scott@leanpub.com |

Yes, that's it!

### Crosslinks

A crosslink lets you refer to another element of your book.  For example, you can refer to another section or figure, even if it's in a different chapter.

Creating crosslinks is a two-step process.

1. You need to set a name for the thing you want to link to.  For example, you can name any chapter or section by putting {#some-name} after the chapter title.

2. You need to link to it in the text.  You do this with a regular Markdown link with a target of "#some-name"; for example [this is a cross-link to the Why Sample Books section in the Sample Books chapter](#why-sample-books).

Note that for standalone crosslinks, this only works when the anchor {#some-name} is at the beginning of a section.

### Footnotes

To add a footnote, you insert it like this[^foo] and then you define the footnote content later.

[^foo]: This is the footnote text.

That's it.  Then you can keep writing content after the footnote content definition as well.
