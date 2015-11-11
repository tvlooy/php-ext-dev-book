# Images and Cover Pages

This chapter shows how you include an image.

![This is the Image Caption](images/LeanpubLogo1200x610_300ppi.png)

That's it!  (See [this web page](http://daringfireball.net/projects/markdown/syntax#img) for details on images in Markdown.)

We support PNG, JPEG and GIF formats for images.

Note that it's important to get the size and the resolution of the image right:

- We use **300 pixels per inch (PPI)** in our books, and we recommend you use that for your images.  Any smaller PPI is scaled up to 300 PPI.  Since we scale up to 300 PPI, your image may look blurry if it's a smaller PPI.

- If you save your image in a 300 PPI format, a 1200 pixel wide image takes 4 inches (1200 pixels / 300 pixel per inch = 4 inches)

- However, if your save your image in a 72 PPI format (the default in most programs), it can only be 288 pixels wide (72 PPI * 4 inches = 288 pixels).  If it's wider, it will bleed into the right margin, and if your image is much too big it may not show up at all.  (Please don't use 72 PPI though, since scaling looks like absolute garbage!)

Different book types have different default paper sizes.  Each different paper size has a different maximum image width, before your image will bleed over the margin.

Currently all Leanpub books have 1 inch margins.

Before we get into specifics there's one really important thing to note.

## Watch Your File Sizes!

Before publishing your book, you should look at the sizes of your images in the images directory.  If your images are huge, your book will be too.  This is especially true at 300 PPI -- the images are a lot bigger than you're used to at 72 PPI.

A simple tip to avoid problems is to use JPEG for all photographs, and to use 80% quality or lower.  For figures that are drawings instead of photographs, PNG will look better than JPEG.  You should be fine with PNG for these, since drawings typically produce files that are a lot smaller than photographs.

Next, here is what you need to know for the different Leanpub book types...

## Technical Books

A technical book defaults to 8.5" x 11" paper.  Subtracting margins, you have 6.5" x 9" to work with.  So your image can be up to 1950 pixels wide and up to 2700 pixels high at 300PPI.  Your cover page uses the full page width, so it should be exactly 2550 pixels wide and 3300 pixels high at 300 PPI.  If this is a technical book, the default title_page.jpg shows you an example of this.

## Business Books

A business book defaults to Digest paper (5.5" x 8").  Subtracting margins, you have 3.5" x 6" to work with.  So your image can be up to 1050 pixels wide and up to 1800 pixels high at 300PPI.  Your cover page uses the full page width, so it should be exactly 1650 pixels wide and 2400 pixels high at 300 PPI.  If this is a business book, the default title_page.jpg shows you an example of this.

## Fiction Books

A business book defaults to Digest paper (5.5" x 8").  Subtracting margins, you have 3.5" x 6" to work with.  So your image can be up to 1050 pixels wide and up to 1800 pixels high at 300PPI.  Your cover page uses the full page width, so it should be exactly 1650 pixels wide and 2400 pixels high at 300 PPI.  If this is a fiction book, the default title_page.jpg shows you an example of this.
