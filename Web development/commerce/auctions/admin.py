from django.contrib import admin

from .models import *

# Register your models here.
class ListingAdmin(admin.ModelAdmin):
    list_display = ('id', 'title', 'starting_bid', 'owner', 'status')


admin.site.register(User)
admin.site.register(AuctionCategory)
admin.site.register(Listing, ListingAdmin)
admin.site.register(Bid)
admin.site.register(Comment)