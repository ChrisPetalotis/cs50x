def layout_parameters(request):
    return {
        'watchlist_length': len(request.user.saved.all()) if request.user.is_authenticated else ''
    }