


/* -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- */
/* Begin Elixir theme specific javascript */
/* -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- */

// 'elixir' is the global object for the Elixir RapidWeaver theme
var elixir = {};

// reduce potential conflicts with other scripts on the page
elixir.jQuery = jQuery.noConflict(true);
var $elixir = elixir.jQuery;

// Create unique object and namespace for theme functions
elixir.themeFunctions = {};

// Define a closure
elixir.themeFunctions = (function() {
    // When jQuery is used it will be available
    // as $ and jQuery but only inside the closure.
    var jQuery = elixir.jQuery;
    var $ = jQuery;
	var $elixir = jQuery.noConflict();


	function blogEntryTopper() {
		/*

		-=-= BLOG ENTRY TOPPER =-=-

		DESCRIPTION: 	Handles the blog entry topper images. Finds image in blog entries with an ALT tag
						of 'blogEntryTopper' and then moves image to the top of the entry. The theme's
						CSS autosizes the image to fit the blog entry's width.

		AUTHOR: Adam Shiver  /  Elixir Graphics
		VERSION: 1.0b
		DATE: May 9, 2011

		*/

		blogEntryTopperImg = $('.blog-entry img[alt^=blogEntryTopper]');
		blogEntryTopperImg.hide();
		blogEntryTopperImg.each(function() {
			/* Applies special class to the image */
			$(this).addClass('blogEntryTopperImg');
			/* Moves topper image to the top of the blog entry and wraps it in a div with a class of blogEntryTopper_wrapper  */
			$(this).prependTo($(this).parent().parent()).show().wrap('<div class="blogEntryTopper_wrapper" />');
		});
	}


	function generalThemeFunctions() {

			$('.blog-read-more').wrap('<div class="read_more_wrapper"/>').find( '> a' ).addClass('btn btn-default');

		if ( $.trim( $('.site_logo').html()).length ) {
		  $('.site_logo, .site_logo img').show().css({'display' : 'inline-block'});
    } else {
			$('.site_logo').remove();
		}

		//
		// Display the Breadcrumb trail only if it is enabled and has content
		//
		if ( $.trim( $('.banner_breadcrumb .container').html()).length ) {
		  $('.banner_breadcrumb').show();
    }

        $('#breadcrumb ul li:nth-child(2) a').contents().unwrap();

		//
		// Add class to last blogn entry on the page. This allows us to target it
        // specifically with some CSS for styling purposes.
		//
    	$('.blog-entry').last().addClass('last-blog-entry');


        //
        // Move social badges to their appropriate locations. While it isn't required,
        // social badges should be placed in the sidebar by the end user. The theme will grab them
        // from other locations on the page though.
        //
        $('a.myBadge').appendTo('#social_badges');

	}


	function navigation() {


		//
		// If a child navigation item in the second tier has children we remove the link from
		// that navigation item, making it a 'dead' link.
		//
		$('#main_navigation > ul > li > a.parent').removeAttr("href").addClass('closed');

		//
		// Handles displaying drop down navigation.
		//
		$('#main_navigation > ul').find("li").each(function() {
				if ($(this).find("ul:first").length > 0) {

						$(this).click(function() {
								//
								// Show child-level menu items
								//
								$(this).find("ul:first").stop(true, true).animate({height: 'toggle' , opacity: 'toggle'}, 250);
								$(this).find("> a").toggleClass('closed').toggleClass('open');
						});
				}
		});


		$('#navigation_toggle').click(function(){
			$('.navigation_overlay').fadeIn(300);
			$('.blur_wrapper').addClass('blur');
			// lock scroll position, but retain settings for later
			var scrollPosition = [
			  self.pageXOffset || document.documentElement.scrollLeft || document.body.scrollLeft,
			  self.pageYOffset || document.documentElement.scrollTop  || document.body.scrollTop
			];

			var html = jQuery('html'); // it would make more sense to apply this to body, but IE7 won't have that
			html.data('scroll-position', scrollPosition);
			html.data('previous-overflow', html.css('overflow'));
			html.css('overflow', 'hidden');
			window.scrollTo(scrollPosition[0], scrollPosition[1]);
		});

		$('#navigation_close_button').click(function(){
			$('.navigation_overlay').fadeOut(225);
			$('.blur_wrapper').removeClass('blur');
			// un-lock scroll position
			var html = jQuery('html');
			var scrollPosition = html.data('scroll-position');
			html.css('overflow', html.data('previous-overflow'));
			window.scrollTo(scrollPosition[0], scrollPosition[1])
		});
	}


	function photoAlbum() {
    //
    // Hanldes caption spacing when image captions are turned on for thumbnails.
    //
		if ($('.thumbnail-wrap .thumbnail-caption').length > 0) {
			$('.thumbnail-wrap').css({'margin-bottom' : '80px'});
		}
	}


	$(document).ready(function() {
  	generalThemeFunctions();
  	navigation();
    photoAlbum();
    blogEntryTopper();
	});


  $(window).load(function(){
    $('.site_navigation').show();
  });

})(elixir.themeFunctions);
