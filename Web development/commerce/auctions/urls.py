from django.urls import path

from . import views

app_name = 'auctions'
urlpatterns = [
    path("", views.index, name="index"),
    path("login", views.login_view, name="login"),
    path("logout", views.logout_view, name="logout"),
    path("register", views.register, name="register"),
    path('listing/<int:listing_id>', views.listing, name='listing'),
    path('create_listing', views.create_listing, name='create_listing'),
    path('watchlist/<int:listing_id>', views.add_to_watchlist, name='add_to_watchlist'),
    path('watchlist', views.watchlist, name='watchlist'),
    path('bid/<int:listing_id>', views.bid, name='bid'),
    path('comment/<int:listing_id>', views.comment, name='comment'),
    path('categories', views.categories, name='categories'),
    path('category/<int:category_id>/listings', views.category_listings, name='category_listings')
]
