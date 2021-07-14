from django.contrib.auth import authenticate, login, logout
from django.db import IntegrityError, transaction
from django.http import HttpResponse, HttpResponseRedirect
from django.shortcuts import render
from django.urls import reverse

from .models import User, Listing, AuctionCategory, Bid, Comment

from decimal import Decimal

def index(request):
    return render(request, "auctions/index.html", {
        'listings': Listing.objects.all()
    })

def listing(request, listing_id):
    listing = Listing.objects.get(pk=listing_id)
    if not listing:
        return HttpResponseRedirect(reverse("auctions:index"))

    if request.method == 'POST':
        # Close the listing, make it inactive and make the highest bidder the winner of the auction
        if not listing.current_highest_bidder:
            return render(request, 'acutions/listing.html', {
                'listing': listing,
                'close_error': 'No one has bid on this auction. Cannot close auction until someone has bid on it.'
            })
        listing.status = False
        listing.winner = listing.current_highest_bidder
        listing.save()
        return HttpResponseRedirect(reverse('auctions:listing', kwargs={'listing_id': listing_id}))
    
    return render(request, 'auctions/listing.html', {
        'listing': listing,
        'in_watchlist': listing in request.user.saved.all(),
        'user_won': (listing.winner == request.user and (not listing.status)),
        'comments': listing.comments.all(),
    })

def create_listing(request):
    if request.method == 'POST':
        title = request.POST['title']
        description = request.POST['description']
        starting_bid = float(request.POST['starting_bid'])
        image_path = request.POST['image_path']
        category = request.POST.get('category', '')
        
        listing = Listing(owner=request.user, title=title, description=description, starting_bid=Decimal(starting_bid), current_price=Decimal(starting_bid), imagePath=image_path)
        listing.save()
        if category:
            auctionCategory = AuctionCategory(name=category)
            auctionCategory.save()
            listing.category = auctionCategory   
            listing.save()

    return render(request, 'auctions/create_listing.html')

def login_view(request):
    if request.method == "POST":

        # Attempt to sign user in
        username = request.POST["username"]
        password = request.POST["password"]
        user = authenticate(request, username=username, password=password)

        # Check if authentication successful
        if user is not None:
            login(request, user)
            return HttpResponseRedirect(reverse("index"))
        else:
            return render(request, "auctions/login.html", {
                "message": "Invalid username and/or password."
            })
    else:
        return render(request, "auctions/login.html")

def logout_view(request):
    logout(request)
    return HttpResponseRedirect(reverse("index"))

def register(request):
    if request.method == "POST":
        username = request.POST["username"]
        email = request.POST["email"]

        # Ensure password matches confirmation
        password = request.POST["password"]
        confirmation = request.POST["confirmation"]
        if password != confirmation:
            return render(request, "auctions/register.html", {
                "message": "Passwords must match."
            })

        # Attempt to create new user
        try:
            user = User.objects.create_user(username, email, password)
            user.save()
        except IntegrityError:
            return render(request, "auctions/register.html", {
                "message": "Username already taken."
            })
        login(request, user)
        return HttpResponseRedirect(reverse("index"))
    else:
        return render(request, "auctions/register.html")

def add_to_watchlist(request, listing_id):
    if request.method == 'POST':
        listing = Listing.objects.get(pk=listing_id)
        if not listing:
            return HttpResponseRedirect(reverse("auctions:index"))

        if listing in request.user.saved.all():
            # Remove listing from user's saved list/watchlist
            request.user.saved.remove(listing)
        else:
            # Add listing to user's saved list/watchlist
            request.user.saved.add(listing)
        
        request.user.save()
        return HttpResponseRedirect(reverse('auctions:listing', kwargs={'listing_id': listing_id}))

def watchlist(request):
    return render(request, 'auctions/watchlist.html', {
        'watchlist': request.user.saved.all()
    })

def bid(request, listing_id):
    if request.method == 'POST':
        listing = Listing.objects.get(pk=listing_id)
        if not listing:
            return HttpResponseRedirect(reverse('auctions:index'))
        
        price = Decimal(request.POST['price'])
        starting_bid = listing.starting_bid
        current_price = listing.current_price
        if price < starting_bid:
            return render(request, 'auctions/listing.html', {
                'listing': listing,
                'bid_message': 'Bidding price should be at least ' + str(starting_bid)
            })
        if price <= current_price:
            return render(request, 'auctions/listing.html', {
                'listing': listing,
                'bid_message': 'Bidding price should be more than ' + str(current_price)
            })

        try:
            with transaction.atomic():
                listing.current_price = price
                listing.current_highest_bidder = request.user
                listing.save()

                bid = Bid(price=price, user=request.user, listing=listing)
                bid.save()
        except IntegrityError:
            return render(request, 'auctions/listing.html', {
                'listing': listing,
                'bid_message': 'There was an error while saving your bid. Please try again.'
            })
        return HttpResponseRedirect(reverse('auctions:listing', kwargs={'listing_id': listing_id}))

def comment(request, listing_id):
    if request.method == 'POST':
        listing = Listing.objects.get(pk=listing_id)
        if not listing:
            return HttpResponseRedirect(reverse("auctions:index"))
        
        commentText = request.POST['comment']
        comment = Comment(text=commentText, user=request.user, listing=listing)
        comment.save()
        return HttpResponseRedirect(reverse('auctions:listing', kwargs={'listing_id': listing_id}))

def categories(request):
    print(AuctionCategory.objects.all())
    return render(request, 'auctions/categories.html', {
        'categories': AuctionCategory.objects.all()
    })

def category_listings(request, category_id):
    category = AuctionCategory.objects.get(pk=category_id)
    if not category:
        return HttpResponseRedirect(reverse("auctions:index"))

    category_listings = category.listings.all()
    return render(request, 'auctions/category_listings.html', {
        'category': category,
        'listings': category_listings
    })
