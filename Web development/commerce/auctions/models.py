from django.contrib.auth.models import AbstractUser
from django.db import models

from datetime import datetime  

class User(AbstractUser):
    pass

class AuctionCategory(models.Model):
    name = models.CharField(max_length=24)

class Listing(models.Model):
    title = models.CharField(max_length=64)
    description = models.CharField(max_length=250)
    starting_bid = models.DecimalField(max_digits=10, decimal_places=2)
    current_price = models.DecimalField(max_digits=12, decimal_places=2)
    current_highest_bidder = models.ForeignKey(User, on_delete=models.PROTECT, related_name='listings_highest', null=True, blank=True)
    owner = models.ForeignKey(User, on_delete=models.PROTECT, related_name='listings')
    imagePath = models.TextField(blank=True)
    creationTime = models.DateTimeField(default=datetime.now, editable=False)
    category = models.ForeignKey(AuctionCategory, on_delete=models.PROTECT, blank=True, null=True, related_name='listings')
    status = models.BooleanField(default=True)
    watchlisters = models.ManyToManyField(User, blank=True, related_name='saved')
    winner = models.ForeignKey(User, on_delete=models.PROTECT, related_name='listings_won', null=True, blank=True)

class Bid(models.Model):
    price = models.DecimalField(max_digits=6, decimal_places=2)
    time = models.DateTimeField(default=datetime.now)
    user = models.ForeignKey(User, on_delete=models.PROTECT, related_name="bids")
    listing = models.ForeignKey(Listing, on_delete=models.PROTECT, related_name='bids')

class Comment(models.Model):
    text = models.CharField(max_length=255)
    time = models.DateTimeField(default=datetime.now)
    user = models.ForeignKey(User, on_delete=models.PROTECT, related_name='comments')
    listing = models.ForeignKey(Listing, on_delete=models.PROTECT, related_name='comments')