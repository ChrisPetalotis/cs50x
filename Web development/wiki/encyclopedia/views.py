from django import forms
from django.http import HttpResponse, HttpResponseRedirect
from django.shortcuts import render
from django.urls import reverse

import random
from markdown2 import Markdown

from . import util

class NewPage(forms.Form):
    title = forms.CharField
    content = forms.CharField

def index(request):
    return render(request, "encyclopedia/index.html", {
        "entries": util.list_entries()
    })

def entry(request, title):
    markdowner = Markdown()

    results = []
    try:
        entry = util.get_entry(title)
        html = markdowner.convert(entry)

        return render(request, "encyclopedia/entry.html", {
            "title": title,
            "content": html,
            "markdown": entry
        })
    except:
        return render(request, "encyclopedia/index.html", {
            "entries": util.list_entries(),
            "errorMessage": "No data for this entry was found"
        })

def search(request):
    searchTerm = request.GET.get('q')
    if util.get_entry(searchTerm):
        return HttpResponseRedirect(reverse("encyclopedia:entry", args=[searchTerm]))
    
    matchingEntries = []
    entries = util.list_entries()
    for entry in entries:
        if searchTerm.lower() in entry.lower():
            matchingEntries.append(entry)
    return render(request, "encyclopedia/searchResults.html", {
        "results": matchingEntries
    })

def newPage(request):
    if request.method == "POST":
        form = request.POST
        title = form.get('title')
        for entry in util.list_entries():
            if title.lower() == entry.lower():
                return render(request, "encyclopedia/newPage.html", {
                    "errorMessage": "An encyclopedia page with this name already exists."
                })
            
        content = form.get('content')
        finalContent = '# ' + title + '\n' + content    
        util.save_entry(title, finalContent)
        return HttpResponseRedirect(reverse("encyclopedia:entry", args=[title]))
    return render(request, "encyclopedia/newPage.html")

def editPage(request):
    if request.method == "POST":
        form = request.POST
        title = form.get('title')
        content = form.get('content')

        util.save_entry(title, content)

        return HttpResponseRedirect(reverse("encyclopedia:entry", args=[title]))
        
    title = request.GET.get('title')
    content = request.GET.get('content')

    return render(request, "encyclopedia/editPage.html", {
        "title": title,
        "content": content
    })

def randomPage(request):
    entries = util.list_entries()
    entries_length = len(entries)
    randomNumber = random.randint(0, entries_length)
    
    return HttpResponseRedirect(reverse("encyclopedia:entry", args=[entries[randomNumber - 1]]))
    